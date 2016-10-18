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

#include <sstmac/hardware/router/fat_tree_dmodk_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#define ftree_dmodk_rter_debug(...) \
  rter_debug("fat tree dmodk: %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fattree_dmodk | ftree_dmodk", router, fat_tree_dmodk_router);

fat_tree_dmodk_router::~fat_tree_dmodk_router()
{
  printf("deleteing fat tree dmodk router\n");
}

void
fat_tree_dmodk_router::productive_paths_to_switch(
  switch_id dst, structured_routable::path_set &paths)
{
  // only 1 possible path
  paths.resize(1);
  minimal_route_to_switch(dst, paths[0]);
}

void
fat_tree_dmodk_router::minimal_route_to_switch(
  switch_id ej_addr,
  structured_routable::path& path)
{
  rter_debug("routing from switch %d:%s -> %d:%s on fat tree router",
    int(my_addr_), top_->switch_coords(my_addr_).to_string().c_str(),
    int(ej_addr), top_->switch_coords(ej_addr).to_string().c_str());

  int pathDim;
  int pathDir = ej_addr % k_;

  // route down
  if (ej_addr >= min_reachable_leaf_id_ && ej_addr < max_reachable_leaf_id_) {
    pathDim = fat_tree::down_dimension;
    path.vc = 1;
    long relative_ej_addr = ej_addr - min_reachable_leaf_id_;
    ftree_dmodk_rter_debug("routing down with dir %d: eject-id=%ld rel-eject-id=%ld",
        pathDir, ej_addr, relative_ej_addr);
  }
  // route up
  else {
    pathDim = fat_tree::up_dimension;
    path.vc = 0;
    ftree_dmodk_rter_debug("routing up with dir %d", pathDir);
  }
  path.outport = regtop_->convert_to_port(pathDim, pathDir);
}

}
}
