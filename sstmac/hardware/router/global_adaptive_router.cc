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

#include <sstmac/hardware/router/global_adaptive_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#include <typeinfo>

#define ftree_rter_debug(...) \
  rter_debug("fat tree (global adaptive): %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("global_adaptive_router", router, global_adaptive_router);

void
global_adaptive_router::route(packet* pkt)
{
    int switch_port; // not used

    // if this switch is the injection switch
    // have to prevent overwriting when new packets come in
    if (top_ -> endpoint_to_injection_switch(pkt -> fromaddr(), switch_port) == my_addr_){
        // get instantaneous queue lengths of the entire network
        interconnect * interconnect = top_ -> get_interconnect();
        switch_interconnect * sw_ic = dynamic_cast <switch_interconnect*> (interconnect);
        const switch_interconnect::switch_map & switches = sw_ic->switches();

        // calculate best route for this packet
        const std::vector <switch_id> path = all_paths(pkt -> fromaddr(), pkt -> toaddr(), switches);

        // modify forwarding tables for each switch on the selected path
        for(std::size_t i = 0; i < path.size() - 1; i++){
            // figure out how to get to path[i + 1] from path[i]
            // router -> set_forwarding_table()
        }
        // egress at end of path
    }
    else{
        // just read from table
        structured_routable* rt = pkt->interface<structured_routable>();
        rt -> current_path().outport = forwarding_table[pkt -> toaddr()].first;
        rt -> current_path().vc = forwarding_table[pkt -> toaddr()].second;
    }
}

void
global_adaptive_router::set_forwarding_table(
    const node_id dst_node,
    const int outport,
    const int vc){
    // set forwarding table
    forwarding_table[dst_node].first = outport;
    forwarding_table[dst_node].second = vc;
}

std::vector <switch_id>
global_adaptive_router::all_paths(
    const node_id src,
    const node_id dst,
    const switch_interconnect::switch_map & switches) const {
    // compute the cheapest route from src to destination given some switch state
    return {};
}

}
}
