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

#include <sstmac/hardware/router/sdn_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/structured_topology.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#define sdn_rter_debug(...) \
  rter_debug("sdn: %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("sdn", router, sdn_router);

void
sdn_router::route(packet* pkt)
{
    geometry_routable* rt = pkt->interface<geometry_routable>();
//    rt -> current_path().outport = rt -> route().front().outport;
//    rt -> current_path().vc      = rt -> route().front().vc;
//    rt -> route().pop_front();
    rt -> current_path().outport = routing::uninitialized;
    rt -> current_path().vc      = 0;
}

}
}
