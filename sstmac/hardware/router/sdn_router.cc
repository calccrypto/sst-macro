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

#include <sstmac/hardware/router/sdn_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/structured_topology.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#define sdn_rter_debug(...) \
  rter_debug("sdn: %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("sdn", router, sdn_router);

sdn_router::~sdn_router()
{
  printf("deleteing sdn router\n");
}

void
sdn_router::add_entry(const int table_id,
                      const Match_Fields & match,
                      const std::vector <Action> & actions)
{
  sdn_router::add_entry(table_id, std::make_pair(match, actions));
}

void
sdn_router::add_entry(const int table_id,
                      const Entry & entry)
{
  if (table_id >= tables.size()){
    tables.resize(table_id + 1);
  }
  tables[table_id].push_back(entry);
}

sdn_router::Match_Fields *
sdn_router::get_packet_metadata(packet * pkt) const
{
  if (!pkt){
      return nullptr;
  }

  Match_Fields * mf = new Match_Fields;
  // get match fields from
  // pkt->interface<structured_routable>()
  return mf;
}

void
sdn_router::route(packet* pkt)
{
  const Match_Fields * packet_fields = get_packet_metadata(pkt);

  if (!packet_fields){
      return;
  }

  // search each table for matches
  bool found = false;
  for(SDN_Table const & table : tables){
    // linearly search each entry in the current table for matches
    for(Entry const & entry : table){
        // if match found, run the actions on the packet
        if (entry.first == *packet_fields){
            // for(auto action : entry.second){
            //   action(&pkt);
            // }
            found = true;
            break;
        }
    }

    if (found){
        break;
    }
  }

  delete packet_fields;
}

}
}
