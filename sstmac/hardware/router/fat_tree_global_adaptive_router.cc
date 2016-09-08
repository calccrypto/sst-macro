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

#include <sstmac/hardware/router/fat_tree_global_adaptive_router.h>
#include <sstmac/hardware/router/routable.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <sprockit/util.h>
#include <sprockit/sim_parameters.h>
#include <cmath>

#define ftree_rter_debug(...) \
  rter_debug("fat tree (global adaptive): %s", sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

SpktRegister("fattree_ga | ftree_ga", router, fat_tree_global_adaptive_router);

fat_tree_global_adaptive_router::~fat_tree_global_adaptive_router()
{
  printf("deleteing fat tree global adaptive router\n");
}

fat_tree_global_adaptive_router::Table
fat_tree_global_adaptive_router::get_table() const
{
  return table;
}

void
fat_tree_global_adaptive_router::set_table(const fat_tree_global_adaptive_router::Table & new_table)
{
  table = new_table;
}

fat_tree_global_adaptive_router::Match_Fields *
fat_tree_global_adaptive_router::get_packet_metadata(packet * pkt) const
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
fat_tree_global_adaptive_router::route(packet* pkt)
{
  const Match_Fields * const packet_fields = get_packet_metadata(pkt);

  if (!packet_fields){
      return;
  }

  for(Entry const & entry : table){
      if (*packet_fields == entry.match_fields){
          structured_routable::path & path = pkt->interface<structured_routable>()->current_path();
          path.vc = entry.vc;
          path.outport = entry.outport;
          break;
      }
  }
}

}
}
