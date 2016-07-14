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
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/fat_tree_dmodk.h>

#define ftree_dmodk_rterrter_debug(...) \
  rter_debug("fat tree dmodk: %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fattree_dmodk", router, fat_tree_dmodk_router);

void
fat_tree_dmodk_router::route(packet* pkt)
{
  dmodk_route_to_node(pkt->toaddr(),
    pkt->interface<routable>()->rinfo().current_path());
}
void
fat_tree_dmodk_router::dmodk_route_to_node(
  node_id dest_node_addr,
  routing_info::path& path)
{
  netlink_id endpoint_id(dest_node_addr / top_->num_nodes_per_netlink());
  switch_id ej_addr = regtop_->endpoint_to_ejection_switch(endpoint_id, path.outport);
  rter_debug("structured router going to switch %d to node %d with eject on %d",
    int(my_addr_), int(dest_node_addr), int(ej_addr));
  if (ej_addr == my_addr_) {
    path.vc = 0;
  }
  else {
    dmodk_route_to_switch(ej_addr, path);
  }
}

void
fat_tree_dmodk_router::dmodk_route_to_switch(
  switch_id ej_addr,
  routing_info::path& path)
{

  rter_debug("routing from switch %d:%s -> %d:%s on fat tree router",
    int(my_addr_), top_->switch_coords(my_addr_).to_string().c_str(),
    int(ej_addr), top_->switch_coords(ej_addr).to_string().c_str());

  int pathDim;
  // int pathDir = ej_addr % k_; // destination mod k
  coordinates dst(2);
  regtop_-> compute_switch_coords(ej_addr, dst);
  int pathDir = dst[1] % k_; // mod k on the column
  int ej_id = ej_addr;

  // route down
  if (ej_id >= min_reachable_leaf_id_ && ej_id < max_reachable_leaf_id_) {
//    pathDim = fat_tree::down_dimension;
    pathDim = dst[0] + 1;
    path.vc = 1;
    long relative_ej_id = ej_id - min_reachable_leaf_id_;
    ftree_dmodk_rterrter_debug("routing down with dir %d: eject-id=%ld rel-eject-id=%ld",
        pathDir, ej_id, relative_ej_id);
  }
  else {
    //route up
//    pathDim = fat_tree::up_dimension;
    pathDim = dst[0] - 1;
    path.vc = 0;
    ftree_dmodk_rterrter_debug("routing up with dir %d", pathDir);
  }
  path.outport = regtop_->convert_to_port(pathDim, pathDir);
}

}
}
