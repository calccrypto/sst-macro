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

fat_tree_local_adaptive_router::~fat_tree_local_adaptive_router()
{
  printf("deleteing fat tree local adaptive router\n");
}

void
fat_tree_local_adaptive_router::minimal_route_to_switch(
  switch_id ej_addr,
  structured_routable::path& path)
{

  rter_debug("routing from switch %d:%s -> %d:%s on fat tree router",
    int(my_addr_), top_->switch_coords(my_addr_).to_string().c_str(),
    int(ej_addr), top_->switch_coords(ej_addr).to_string().c_str());

  // get all possible paths
  structured_routable::path_set paths;
  productive_paths_to_switch(ej_addr, paths);

  // find cheapest
  path = paths[0];
  int cost = INT_MAX;       // need global stat here
  for(int i = 1; i < paths.size(); i++){
    int new_cost = INT_MAX; // need global stat here
    if (new_cost < cost){
        path = paths[i];
        cost = new_cost;
    }
  }
}

}
}
