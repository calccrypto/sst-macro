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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREE_DMODK_ROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREE_DMODK_ROUTER_H_INCLUDED

#include <sstmac/hardware/router/fat_tree_router.h>

namespace sstmac {
namespace hw {

class fat_tree_dmodk_router :
  public fat_tree_router
{
 public:
  virtual
  ~fat_tree_dmodk_router() {
  }

  virtual std::string
  to_string() const {
    return "fattreedmodkrouter";
  }

  virtual void
  route(packet* pkt);

 protected:
  void
  dmodk_route_to_node(
    node_id node_addr,
    routing_info::path& current_path);

  void
  dmodk_route_to_switch(
    switch_id sw_addr,
    routing_info::path& path);
};



}
}
#endif
