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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREEGLOBALADAPTIVEROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREEGLOBALADAPTIVEROUTER_H_INCLUDED

#include <sstmac/hardware/router/structured_router.h>
#include <sstmac/hardware/interconnect/switch_interconnect.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <list>

namespace sstmac {
namespace hw {

class fat_tree_global_adaptive_router :
  public structured_router
{
 public:
  virtual ~fat_tree_global_adaptive_router() {}

  fat_tree_global_adaptive_router() :
      structured_router(routing::minimal_adaptive) {}

  virtual std::string
  to_string() const {
    return "fat tree global adaptive router";
  }

  virtual void
  route(packet* pkt);

  virtual void
  set_topology(topology * top){
      if (!test_cast(fat_tree, top)){
          spkt_throw(sprockit::value_error, "topology is not fat tree");
      }
      top_ = top;
  }

 private:
  void
  all_paths(
    const switch_id src,
    const switch_id dst,
    fat_tree::dimension_t dim,
    std::list <structured_routable::path> & path,
    std::list <std::list <structured_routable::path> > & paths,
    fat_tree * ftree) const;

  std::list <structured_routable::path>
  cheapest_path(
    const node_id src,
    const node_id dst,
    const switch_interconnect::switch_map & switches) const;
};

}
}
#endif
