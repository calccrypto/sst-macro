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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_GLOBALADAPTIVEROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_GLOBALADAPTIVEROUTER_H_INCLUDED

#include <sstmac/hardware/router/structured_router.h>
#include <sstmac/hardware/interconnect/switch_interconnect.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <sprockit/util.h>

#include <list>

namespace sstmac {
namespace hw {

class global_adaptive_router :
  public structured_router
{
 public:
  virtual ~global_adaptive_router() {}

  global_adaptive_router() :
      structured_router(routing::minimal) {}

  virtual std::string
  to_string() const {
    return "global adaptive";
  }

  virtual void
  route(packet* pkt);

  virtual void
  set_topology(topology * top){
      if (test_cast(fat_tree, top)){
          spkt_throw(sprockit::value_error, "use fat_tree_global_adaptive for with fat tree topologies");
      }
      top_ = top;
  }

 private:
  std::list <geometry_routable::path>
  all_paths(
    const node_id src,
    const node_id dst,
    const switch_interconnect::switch_map & switches) const;
};

}
}
#endif
