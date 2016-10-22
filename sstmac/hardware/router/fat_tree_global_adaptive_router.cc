/*
 *  This file is part of SST/macroscale:
 *               The macroscale architecture simulator from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top
 *  SST/macroscale directory.
 */

#include <sstmac/hardware/router/fat_tree_global_adaptive_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sprockit/sim_parameters.h>

#include <climits>

#define ftree_ga_rter_debug(...) \
  rter_debug("fat tree (global adaptive): %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fat_tree_global_adaptive", router, fat_tree_global_adaptive_router);

void
fat_tree_global_adaptive_router::route(packet* pkt)
{
    int switch_port; // not used
    structured_routable * rt = pkt -> interface<structured_routable>();

    // if this switch is the injection switch
    if (top_ -> endpoint_to_injection_switch(pkt -> fromaddr(), switch_port) == my_addr_){
        // get instantaneous queue lengths of the entire network
        interconnect * inter = top_ -> get_interconnect();
        switch_interconnect * sw_ic = dynamic_cast <switch_interconnect *> (inter);
        const switch_interconnect::switch_map & switches = sw_ic->switches();

        // calculate best route for this packet
        rt -> route() = cheapest_path(pkt -> fromaddr(), pkt -> toaddr(), switches);
    }

    // normally no need to check route length since there are always values
    if (!rt -> route().size()){
        rt -> current_path().outport = routing::uninitialized;
        rt -> current_path().vc      = 0;
        return;
    }

    rt -> current_path().outport = rt -> route().front().outport;
    rt -> current_path().vc      = rt -> route().front().vc;
    rt -> route().pop_front();
}

void
fat_tree_global_adaptive_router::all_paths(
    const switch_id src,
    const switch_id dst,
    fat_tree::dimension_t dim,
    std::list <structured_routable::path> & path,
    std::list <std::list <structured_routable::path> > & paths,
    fat_tree * ftree){

    // save good path
    if (src == dst){
        paths.push_back(path);
        return;
    }

    coordinates src_coor(2);
    ftree -> compute_switch_coords(src, src_coor);

    coordinates dst_coor(2);
    ftree -> compute_switch_coords(dst, dst_coor);

    // really bad fix here; not sure what the real fix should be
    if (src >= ftree -> num_switches()){
        return;
    }

    // ignore bad path (same level but different switch)
    if ((dim == fat_tree::down_dimension) && (src_coor[0] == dst_coor[0])){
        return;
    }

    // if src is at the middle hop, change the direction
    // since routing is up -> down, change it to down
    if (ftree -> nearest_common_ancestor_level(src, dst) == src_coor[0]){
        dim = fat_tree::down_dimension;
    }

    // try all ports
    for(int k = 0; k < ftree -> k(); k++){
        structured_routable::path p;
        p.outport = ftree -> convert_to_port(dim, k);
        p.vc = dim;
        path.push_back(p);
        all_paths(ftree -> switch_number(ftree -> neighbor_at_port(src, p.outport)), dst, dim, path, paths, ftree);
        path.pop_back();
    }
}

// compute the cheapest route from src to destination given some switch state
std::list <structured_routable::path>
fat_tree_global_adaptive_router::cheapest_path(
    const node_id src,
    const node_id dst,
    const switch_interconnect::switch_map & switches){

    // get source and destination switches
    int outport;
    switch_id src_sw = top_ -> endpoint_to_injection_switch(src, outport); // ignore this outport
    switch_id dst_sw = top_ -> endpoint_to_ejection_switch (dst, outport);

    fat_tree * ftree = test_cast(fat_tree, top_);

    // generate all valid paths
    std::list <structured_routable::path> path;
    std::list <std::list <structured_routable::path> > paths;
    all_paths(src_sw, dst_sw, fat_tree::up_dimension, path, paths, ftree);

    // find path with minimum queue length
    int queue_length = INT_MAX;
    std::list <structured_routable::path> cheapest;
    for(std::list <structured_routable::path> const & p : paths){
        int this_queue_length = 0;

        // add up queue lengths
        for(structured_routable::path const & hop : p){
            // use instantaneous queue lengths
            this_queue_length += switches.at(src_sw) -> queue_length(hop.outport);
            coordinates src_coor = ftree -> neighbor_at_port(src_sw, hop.outport);
            src_sw = ftree -> switch_number(src_coor);
        }

        if (this_queue_length < queue_length){
            queue_length = this_queue_length;
            cheapest = p;
        }
    }

    // add ejection port
    structured_routable::path p;
    p.outport = outport;
    p.vc = 1;
    cheapest.push_back(p);

    return cheapest;
}

}
}
