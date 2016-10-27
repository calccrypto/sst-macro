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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_LOCALADAPTIVEROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_LOCALADAPTIVEROUTER_H_INCLUDED

#include <sstmac/hardware/router/minad_routing.h>
#include <sstmac/common/rng.h>

namespace sstmac {
namespace hw {

class local_adaptive_router :
  public minimal_adaptive_router
{
 public:
  virtual ~local_adaptive_router() {}

  local_adaptive_router() :
      minimal_adaptive_router()
  {
      algo_ = routing::minimal;
  }

  virtual std::string
  to_string() const {
    return "local adaptive";
  }

  // override minimal_router's set_topology check
  virtual void
  set_topology(topology *top);
};

}
}
#endif
