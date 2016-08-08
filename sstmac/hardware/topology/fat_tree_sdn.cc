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
  // assume current_sw_addr is the source switch
  if (!path.chosen.size()){
    // check flow table first
    Match_Fields mf;
    mf.src = current_sw_addr;
    mf.dst = dest_sw_addr;

    // find route in flow table
    Flow_Table::iterator it = flow_table.find(mf);

    // if flow not found, generate one and add it to the table
    if (it == flow_table.end()){
      const int ncal = nearest_common_ancestor_level(current_sw_addr, dest_sw_addr);

      // insert flow entry into the table
      // dont use operator[]; there is a weird bug
      it = flow_table.insert(std::make_pair (mf, Path((ncal << 1) + 1))).first;
      (it -> second)[0] = geometry_routable::path::Hop(current_sw_addr, -1, 0);

      // calculate and store path
      cheapest_path(0, ncal, dest_sw_addr, it -> second);
    }

    // path should not be stored in path.chosen
    path.chosen = it -> second;
  }

  // look up route and figure out where to go next
  bool found = false;
  // this should be looping on flow table entry, not path stored in path.chosen
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
