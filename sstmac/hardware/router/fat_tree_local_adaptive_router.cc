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
#include <sstmac/hardware/topology/fat_tree.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>
#include <climits>

#define ftree_la_rter_debug(...) \
  rter_debug("fat tree (local adaptive): %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fattree_la | ftree_la", router, fat_tree_local_adaptive_router);

// copied from minimal_adaptive_router::route
void
fat_tree_local_adaptive_router::route(packet* pkt)
{
  structured_routable* rt = pkt->interface<structured_routable>();
  structured_routable::path_set paths;
  bool eject  = productive_paths_to_node(pkt->toaddr(), paths);
  if (eject) {
    rt->assign_path(paths[0]);
    return;
  }
  //loop through the ports and find the least-congested
  int min_queue_length = netsw_->queue_length(paths[0].outport);
  structured_routable::path& min_path = paths[0];
  debug_printf(sprockit::dbg::router,
    "Routing %p from %ld to %ld: path 0 port=%d queue=%d",
    pkt,
    long(netsw_->addr()),
    long(pkt->toaddr()),
    paths[0].outport,
    min_queue_length);

  for (int i=1; i < paths.size(); ++i) {
    int test_length = netsw_->queue_length(paths[i].outport);
    debug_printf(sprockit::dbg::router,
      "  path %d: port=%d queue=%d",
      i, paths[i].outport, test_length);
    if (test_length < min_queue_length) {
      min_path = paths[i];
      min_queue_length = test_length;
    }
  }
  debug_printf(sprockit::dbg::router,
    "  chose %d", min_path.outport);
  rt->assign_path(min_path);
}

}
}
