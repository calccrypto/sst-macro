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
fat_tree_global_adaptive_router::set_topology(topology * top){
    if (!test_cast(fat_tree, top)){
      spkt_throw(sprockit::value_error, "topology is not fat tree");
    }

    fat_tree_router::set_topology(top);
}

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

    rt -> current_path().outport = rt -> route().front().outport;
    rt -> current_path().vc      = rt -> route().front().vc;
    rt -> route().pop_front();
}

// be careful when changing ordering of if statements
void
fat_tree_global_adaptive_router::all_paths(
    const switch_id src,
    const switch_id dst,
    const coordinates & dst_coor,  // no point in calculating this multiple times
    fat_tree::dimension_t dim,
    std::list <structured_routable::path> & path,
    std::list <std::list <structured_routable::path> > & paths,
    fat_tree * ftree,
    const int num_switches){       // no point in calculating this multiple times

    // save good path
    if (src == dst){
        paths.push_back(path);
        return;
    }

    coordinates src_coor(2);
    ftree -> compute_switch_coords(src, src_coor);

    // ignore bad path (same level but different switch)
    // don't need to check column since that's implicitly done in the previous check
    if ((dim == fat_tree::down_dimension) && (src_coor[0] == dst_coor[0])){
        return;
    }

    // really bad fix here; not sure what the real fix should be
    if (src >= num_switches){
        return;
    }

    if (dim == fat_tree::up_dimension){
        // if current switch is at the nearest common ancestor level hop and src is not the leaf switch, change the direction
        // since routing is up -> down, change it to down
        if ((ftree -> nearest_common_ancestor_level(src, dst) < 2) && (src_coor[0] != 0)){
            dim = fat_tree::down_dimension;
        }

        // try all ports
        for(int k = 0; k < ftree -> k(); k++){
            structured_routable::path p;
            p.outport = ftree -> convert_to_port(dim, k);
            p.vc = dim;
            path.push_back(p);
            all_paths(ftree -> switch_number(ftree -> neighbor_at_port(src, p.outport)), dst, dst_coor, dim, path, paths, ftree, num_switches);
            path.pop_back();
        }
    }
    else{
        // single route down
        structured_routable::path p;

        // if current switch is 2 levels or more above the destination switch, divide to get port
        if (src_coor[0] > (dst_coor[0] + 1)){
            p.outport = ftree -> convert_to_port(fat_tree::down_dimension, dst / ftree -> k());
        }
        // if the current switch is 1 level above the destination switch, just mod
        else{
            p.outport = ftree -> convert_to_port(fat_tree::down_dimension, dst % ftree -> k());
        }

        p.vc = fat_tree::down_dimension;
        path.push_back(p);
        all_paths(ftree -> switch_number(ftree -> neighbor_at_port(src, p.outport)), dst, dst_coor, fat_tree::down_dimension, path, paths, ftree, num_switches);
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

    // cache dst_coor value
    coordinates dst_coor(2);
    ftree -> compute_switch_coords(dst_sw, dst_coor);

    // generate all valid paths
    std::list <structured_routable::path> path;
    std::list <std::list <structured_routable::path> > paths;
    all_paths(src_sw, dst_sw, dst_coor, fat_tree::up_dimension, path, paths, ftree, ftree -> num_switches());

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
