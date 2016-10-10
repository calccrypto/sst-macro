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

#include <vector>

namespace sstmac {
namespace hw {

/**
 * @brief The sdn_router class
 * Router encapsulating the special routing computations that must occur on
 * a fat tree topology.
 */
class sdn_router :
  public structured_router
{
 public:
  virtual ~sdn_router();

  sdn_router() :
      structured_router(routing::minimal)
  {}

  virtual std::string
  to_string() const {
    return "sdnrouter";
  }

  // some subset of Packet
  struct Match_Fields{
    int flow_id;
    int src;
    int dst;

    bool operator==(const Match_Fields & mf) const {
      return ((flow_id == mf.flow_id) &&
              (src == mf.src) &&
              (dst == mf.dst));
    }
  };

  typedef void * Action;                                         // functor/std::function/lambda
  typedef std::pair <Match_Fields, std::vector <Action> > Entry; // a single entry with its set of actions
  typedef std::vector <Entry> SDN_Table;                         // a single table of multiple matching patterns and associated actions

  // add entry to table
  void
  add_entry(const int table_id,
            const Match_Fields & match,
            const std::vector <Action> & actions);

  // add entry to table
  void
  add_entry(const int table_id,
            const Entry & entry);

  // no delete/remove functions provided

 private:
  Match_Fields *
  get_packet_metadata(packet * pkt) const;

 public:
  virtual void
  route(packet* pkt);

 protected:
  sdn_router(routing::algorithm_t algo) :
      structured_router(algo){}

 private:
  std::vector <SDN_Table> tables;                                // flow tables
};

}
}
#endif
