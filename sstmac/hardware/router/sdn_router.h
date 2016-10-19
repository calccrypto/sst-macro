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
#include <sstmac/software/process/app_id.h>

#include <map>
#include <vector>

namespace sstmac {
namespace hw {

class sdn_router :
  public structured_router
{
 public:
  virtual ~sdn_router() {}

  sdn_router() :
      structured_router(routing::sdn) {}

  virtual std::string
  to_string() const {
    return "sdn router";
  }

  void
  route(packet* pkt);

  private:
    // flow_table[app_id][src][dst] -> {path}
    std::map <sw::app_id, std::map <node_id, std::map <node_id, std::vector <switch_id> > > > flow_table;
};

}
}
#endif
