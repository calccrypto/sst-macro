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

#include <sstmac/hardware/topology/fat_tree_global_adaptive.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_global_adaptive", topology, fat_tree_global_adaptive,
             "Fat tree topology with L levels, radix K, and GLOBAL_ADAPTIVE routing.");

void
fat_tree_global_adaptive::connect_objects(internal_connectable_map& switches){
    fat_tree::connect_objects(switches);
    switch_copy = switches; // save a copy of the switches
}

// global_adaptive routing
void
fat_tree_global_adaptive::global_adaptive(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path & path) const
{
  // if path has not been selected, select one before pushing to outport
  if (!path.chosen.size()){
      // assume current_sw_addr is the source switch
      set_path(current_sw_addr, dest_sw_addr, path);
  }

  // get current switch coordinates
  coordinates curr(2);
  compute_switch_coords(current_sw_addr, curr);

  // linear search on path
  bool found = false;
  for(routing_info::path::Hop const & p : path.chosen){
      if (p.sw_id == current_sw_addr){
          path.outport = p.outport;
          path.vc = p.vc;
          found = true;
          break;
      }
  }

  if (!found){
      spkt_throw_printf(sprockit::value_error, "fat_tree: global_adaptive routing did not find route to next switch");
  }

  top_debug("fat_tree: global_adaptive routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_global_adaptive::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path& path) const
{
  global_adaptive(current_sw_addr, dest_sw_addr, path);
}

void
fat_tree_global_adaptive::set_path(
  switch_id src,
  switch_id dst,
  routing_info::path & path) const
{
    cheapest_path(src, dst, path.chosen);
}

void
fat_tree_global_adaptive::cheapest_path(
      switch_id src,
      switch_id dst,
      std::vector <routing_info::path::Hop> & path) const
{

//    // start at source
//    path.chosen.push_back(routing_info::path::Hop(src, -1, 0));

//    // fill in middle

//    // end at dest (really don't need dst, since route function shouldnt be called at destination)
//    path.chosen.push_back(routing_info::path::Hop(dst, -1, 1));

}

}
} //end of namespace sstmac
