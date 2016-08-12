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
             "Fat tree topology with L levels, radix K, and global adaptive routing.");

void
fat_tree_global_adaptive::cheapest_path(
    const std::size_t current_index,
    const std::size_t midpoint,
    const switch_id dst,
    Path & current,                 // current path
    const std::size_t current_cost, // cost of current path
    Path & path,                    // cheapest path
    std::size_t & path_cost) const  // cost of cheapest path, initialized to INT_MAX
{
  // at the end of the path; only allow paths that reach destination to be chosen from
  if ((current_index + 1) == current.size()){
    if (current.back().sw_id == dst){
      if (current_cost < path_cost){
        path = current;
        path_cost = current_cost;
      }
    }
    return;
  }

  int offset;
  if (current_index < midpoint){                // before midpoint
    current[current_index].vc = up_dimension;   // keep going up
    offset = k_;                                // up ports are [k_, 2 k_)
  }
  else{                                         // at or after midpoint
    current[current_index].vc = down_dimension; // go downwards
    offset = 0;                                 // down ports are [0, k_)
  }

  // depth first search through ports for cheapest path to destination
  int outport = -1;
  int cheapest = INT_MAX;
  for(int k = 0; k < k_; k++){
    // set current outport to the port
    current[current_index].outport = k + offset;

    // set next switch id to the switch that is the neighbor at the port
    current[current_index + 1].sw_id = switch_number(neighbor_at_port(current[current_index].sw_id, current[current_index].outport));

    // cost from current switch to the switch down the current port
    const std::size_t cost_to_next_hop = 0;

    // cost of path after this hop
    cheapest_path(current_index + 1, midpoint, dst, current, current_cost + cost_to_next_hop, path, path_cost);
  }

  return;
}

// global_adaptive routing
// each packet gets its own route
void
fat_tree_global_adaptive::global_adaptive(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path & path) const
{
  // if path has not been selected, select one before pushing to outport
  // assume current_sw_addr is the source switch
  if (!path.chosen.size()){
    const int ncal = nearest_common_ancestor_level(current_sw_addr, dest_sw_addr);

    std::vector <geometry_routable::path::Hop> temp((ncal << 1) + 1);
    temp[0] = geometry_routable::path::Hop(current_sw_addr, -1, 0);

    // find the cheapest path and set it to the packet path
    std::size_t path_cost = INT_MAX;
    cheapest_path(0, ncal, dest_sw_addr, temp, 0, path.chosen, path_cost);
  }
  // chose current path
  path.outport = path.chosen[path.index].outport;
  path.vc = path.chosen[path.index].vc;
  path.index++;

  // use this for error checking
  // // linear search on path
  // bool found = false;
  // for(geometry_routable::path::Hop const & p : path.chosen){
  //   if (p.sw_id == current_sw_addr){
  //     path.outport = p.outport;
  //     path.vc = p.vc;
  //     found = true;
  //     break;
  //   }
  // }

  // if (!found){
  //   spkt_throw_printf(sprockit::value_error, "fat tree: global adaptive routing did not find route to next switch");
  // }

  top_debug("fat_tree: global adaptive routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_global_adaptive::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path& path)
{
  global_adaptive(current_sw_addr, dest_sw_addr, path);
}

}
} //end of namespace sstmac
