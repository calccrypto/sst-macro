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
    const sw::app_id app = 0;//pkt -> appid();
    const node_id src = pkt -> fromaddr();
    const node_id dst = pkt -> toaddr();

    structured_routable* rt = pkt->interface<structured_routable>();

    // just read from table
    rt -> current_path().outport = flow_table[app][src][dst].first;
    rt -> current_path().vc      = flow_table[app][src][dst].second;
}

void
sdn_router::set_flow(
    const sw::app_id aid,
    const node_id src,
    const node_id dst,
    const int outport,
    const int vc){

    flow_table[aid][src][dst].first = outport;
    flow_table[aid][src][dst].second = vc;
}

}
}
