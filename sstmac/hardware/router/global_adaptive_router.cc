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
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#include <typeinfo>

#define ga_rter_debug(...) \
  rter_debug("global adaptive router: %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("global_adaptive_router", router, global_adaptive_router);

void
global_adaptive_router::route(packet* pkt)
{
    int switch_port; // not used
    structured_routable* rt = pkt->interface<structured_routable>();

    // if this switch is the injection switch
    if (top_ -> endpoint_to_injection_switch(pkt -> fromaddr(), switch_port) == my_addr_){
        // get instantaneous queue lengths of the entire network
        interconnect * interconnect = top_ -> get_interconnect();
        switch_interconnect * sw_ic = dynamic_cast <switch_interconnect*> (interconnect);
        const switch_interconnect::switch_map & switches = sw_ic->switches();

        // calculate best route for this packet
        rt -> route() = all_paths(pkt -> fromaddr(), pkt -> toaddr(), switches);
    }
    else{
        // normally no need to check route length since there are always values
        if (!rt -> route().size()){
            rt -> current_path().outport = routing::uninitialized;
            rt -> current_path().vc      = 0;
            return;
        }

        rt -> current_path().outport = rt -> route().front().outport;
        rt -> current_path().vc      = rt -> route().front().outport;
        rt -> route().pop_front();
    }
}

std::list <structured_routable::path>
global_adaptive_router::all_paths(
    const node_id src,
    const node_id dst,
    const switch_interconnect::switch_map & switches) const {
    // compute the cheapest route from src to destination given some switch state
    return {};
}

}
}
