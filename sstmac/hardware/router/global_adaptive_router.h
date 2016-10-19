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
#include <vector>

namespace sstmac {
namespace hw {

class global_adaptive_router :
  public structured_router
{
 public:
  virtual ~global_adaptive_router() {}

  global_adaptive_router() :
      structured_router(routing::minimal_adaptive)
  {}

  virtual std::string
  to_string() const {
    return "global adaptive router";
  }

  virtual void
  route(packet* pkt);

  void
  set_forwarding_table(
    const node_id dst_node,
    const int outport,
    const int vc);

 private:
  std::vector <switch_id>
  all_paths(
    const node_id src,
    const node_id dst,
    const switch_interconnect::switch_map & switches) const;

  // forwarding table
  // destination from this router -> outport, vc
  std::map <node_id, std::pair <int, int> > forwarding_table;
};

}
}
#endif
