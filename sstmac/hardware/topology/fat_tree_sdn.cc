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

#include <sstmac/hardware/topology/fat_tree_sdn.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_sdn", topology, fat_tree_sdn,
             "Fat tree topology with L levels, radix K, and sdn routing.");

// sdn routing
// route flows, not individual packets
void
fat_tree_sdn::sdn(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path & path)
{
  // if path has not been selected, select one before pushing to outport
  // assume current_sw_addr is the source switch
  if (!path.chosen.size()){
      // check flow table first
      Match_Fields mf;
      mf.src = current_sw_addr;
      mf.dst = dest_sw_addr;

      // if route not found, generate one and put it into the flow table
      Flow_Table::iterator it = flow_table.find(mf);
      if (it == flow_table.end()){
          const int ncal = nearest_common_ancestor_level(current_sw_addr, dest_sw_addr);
          Path chosen((ncal << 1) + 1);
          path.chosen.front() = geometry_routable::path::Hop(current_sw_addr, -1, 0);
          cheapest_path(0, ncal, dest_sw_addr, chosen);
          it = flow_table.insert(std::make_pair(mf, chosen)).first;
      }

      // set the packet's route
      path.chosen = it -> second;
  }

  // linear search on path
  bool found = false;
  for(geometry_routable::path::Hop const & p : path.chosen){
      if (p.sw_id == current_sw_addr){
          path.outport = p.outport;
          path.vc = p.vc;
          found = true;
          break;
      }
  }

  if (!found){
      spkt_throw_printf(sprockit::value_error, "fat_tree: sdn routing did not find route to next switch");
  }

  top_debug("fat_tree: sdn routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_sdn::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path& path)
{
  sdn(current_sw_addr, dest_sw_addr, path);
}

}
} //end of namespace sstmac
