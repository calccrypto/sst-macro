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

void
fat_tree_sdn_router::add_entry(const int table_id,
                               const Match_Fields & entry)
{
  if (table_id >= tables.size()){
    tables.resize(table_id + 1);
  }
  tables[table_id].first.insert(entry);
}

void
fat_tree_sdn_router::add_action(const int table_id,
                                const Action & action,
                                const std::string & name)
{
  if (table_id >= tables.size()){
    tables.resize(table_id + 1);
  }
  tables[table_id].second.insert(std::make_pair(action, name));
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
    Entries::const_iterator it = table.first.find(*packet_fields);

    // if match found, run the actions on the packet
    if (it != table.first.end()){
      // for(std::function & action : table.second){
      //   action(&pkt);
      // }
      break;
    }
  }

  delete packet_fields;
}

}
}
