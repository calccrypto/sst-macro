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

#include <sstmac/hardware/router/fat_tree_router.h>

namespace sstmac {
namespace hw {

/**
 * @brief The fat_tree_sdn_router class
 * Router encapsulating the special routing computations that must occur on
 * a fat tree topology.
 */
class fat_tree_sdn_router :
  public fat_tree_router
{
 public:
  virtual ~fat_tree_sdn_router();

  fat_tree_sdn_router() :
      fat_tree_router()
  {
  }

  virtual std::string
  to_string() const {
    return "fattreesdnrouter";
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

  typedef std::list <Match_Fields> Entries;                     // ordered set of all patterns to match against
  typedef void * Action;                                        // functors/std::function
  typedef std::list <std::pair <Action, std::string> > Actions; // actions and some unique id
  typedef std::pair <Entries, Actions> SDN_Table;               // a single table of multiple matching patterns and associated actions

  // get entries from a table to modify
  Entries
  get_entries(const int table_id) const;

  // get actions from a table to modify
  Actions
  get_actions(const int table_id) const;

  // set the entire entry list of a table
  void
  set_entries(const int table_id,
              const Entries & entries);

  // set the entire action list of a table
  void
  set_actions(const int table_id,
              const Actions & actions);

 private:
  Match_Fields *
  get_packet_metadata(packet * pkt) const;

 public:
  virtual void
  route(packet* pkt);

 private:
  std::vector <SDN_Table> tables;
};

}
}
#endif
