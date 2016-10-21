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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREESDNROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREESDNROUTER_H_INCLUDED

#include <sstmac/hardware/router/structured_router.h>
#include <sstmac/hardware/topology/coordinates.h>

namespace sstmac {
namespace hw {

// routes for packets that travel through these routers are pre established by the launcher

class sdn_router :
  public structured_router
{
 public:
  virtual ~sdn_router() {}

  sdn_router() :
      structured_router(routing::minimal) {}

  virtual std::string
  to_string() const {
    return "sdn router";
  }

  void
  route(packet* pkt);
};

}
}
#endif
