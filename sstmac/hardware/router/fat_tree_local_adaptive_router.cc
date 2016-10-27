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

#include <sstmac/hardware/router/fat_tree_local_adaptive_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sprockit/sim_parameters.h>

#define ftree_la_rter_debug(...) \
  rter_debug("fat tree (global adaptive): %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fat_tree_local_adaptive", router, fat_tree_local_adaptive_router);

void
fat_tree_local_adaptive_router::set_topology(topology * top){
    if (!test_cast(fat_tree, top)){
      spkt_throw(sprockit::value_error, "topology is not fat tree");
    }

    fat_tree_router::set_topology(top);
}

void
fat_tree_local_adaptive_router::route(packet* pkt)
{
    geometry_routable * rt = pkt -> interface<geometry_routable>();

    // get src and dst switch ids
    int outport;
    const switch_id src_sw = top_ -> endpoint_to_injection_switch(rt -> fromaddr(), outport); // ignore this outport
    const switch_id dst_sw = top_ -> endpoint_to_ejection_switch (rt -> toaddr(),   outport);

    // if this is the ejection switch, eject
    if (dst_sw == my_addr_){
        rt -> current_path().outport = outport;
        rt -> current_path().vc = 1;
        return;
    }

    // get all ports that get packet closer to destination
    geometry_routable::path_set paths;
    productive_paths_to_switch(dst_sw, paths);

    // check queue length of all good ports
    rt -> current_path() = paths[0];
    for(int i = 1; i < paths.size(); i++){
        if (netsw_ -> queue_length(rt -> current_path().outport) > netsw_ -> queue_length(paths[i].outport)){
            rt -> current_path() = paths[i];
        }
    }
}

}
}
