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

#include <sstmac/hardware/router/fat_tree_sdn_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#define ftree_sdn_rter_debug(...) \
  rter_debug("fat tree (sdn): %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fattree_sdn | ftree_sdn", router, fat_tree_sdn_router);

fat_tree_sdn_router::~fat_tree_sdn_router()
{
  printf("deleteing fat tree sdn router\n");
}

// get entries from a table to modify
fat_tree_sdn_router::Entries
fat_tree_sdn_router::get_entries(const int table_id) const
{
  if (table_id >= tables.size()){
    return {};
  }
  return tables[table_id].first; // return modifiable data
}

// get actions from a table to modify
fat_tree_sdn_router::Actions
fat_tree_sdn_router::get_actions(const int table_id) const
{
  if (table_id >= tables.size()){
      return {};
  }
  return tables[table_id].second; // return modifiable data
}

void
fat_tree_sdn_router::set_entries(const int table_id,
                                 const Entries & entries)
{
  if (table_id < tables.size()){
    tables[table_id].first = entries;
  }
}

void
fat_tree_sdn_router::set_actions(const int table_id,
                                const Actions & actions)
{
  if (table_id < tables.size()){
    tables[table_id].second = actions;
  }
}

fat_tree_sdn_router::Match_Fields *
fat_tree_sdn_router::get_packet_metadata(packet * pkt) const
{
  if (!pkt){
      return nullptr;
  }

  // do something with
  // pkt->interface<structured_routable>()
  Match_Fields * mf = new Match_Fields;
  return mf;
}

void
fat_tree_sdn_router::route(packet* pkt)
{
  const Match_Fields * const packet_fields = get_packet_metadata(pkt);

  if (!packet_fields){
      return;
  }

  // search each table for matches
  for(SDN_Table const & table : tables){
    // search each entry in the current table for matches
    for(Match_Fields const & match_field: table.first){
      // if match found, run the actions on the packet
      if (*packet_fields == match_field){
        // for(std::function & action : sdn_table.second){
        //   action(&pkt);
        // }
        break;
      }
    }
  }

  delete packet_fields;
}

}
}
