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

unsigned int
fat_tree_global_adaptive::cheapest_path(
    const std::size_t current_index,
    const unsigned int current_cost,
    const std::size_t mid_point,
    const switch_id dst,
    std::vector <routing_info::path::Hop> & path) const
{
    // at the end of the path; only allow paths that reach destination to be chosen from
    if ((current_index + 1) == path.size()){
        return (path.back().sw_id == dst)?
                                          0:       // this should be cost to get to the destination
                                          INT_MAX;
    }

    int offset;
    if (current_index < mid_point){
        path[current_index].vc = 0; // if the current index is before the mid point, keep going up
        offset = k_;                // up ports are [k_, 2 k_)
    }
    else{
        path[current_index].vc = 1; // otherwise, go downwards
        offset = 0;                 // down ports are [0, k_)
    }

    // depth first search through ports for cheapest path to destination
    int outport = -1;
    int cheapest = INT_MAX;
    for(int k = 0; k < k_; k++){
        // set current outport to the port
        path[current_index].outport = k + offset;

        // set next switch id to the switch that is the neighbor at the port
        path[current_index + 1].sw_id = switch_number(neighbor_at_port(path[current_index].sw_id, path[current_index].outport));

        // get cost to get to next switch
        const int this_hop_cost = 0;

        // get cheapest cost going down this port at this switch
        const int next_hop_cost = cheapest_path(current_index + 1, current_cost + this_hop_cost, mid_point, dst, path);

        // compare costs of going down each port at this switch
        if (next_hop_cost < cheapest){
            outport = k + offset;
            cheapest = next_hop_cost;
        }
    }

    // set the path to go out the cheapest port, overwriting recursion data
    path[current_index + 1].sw_id = switch_number(neighbor_at_port(path[current_index].sw_id, outport));
    path[current_index].outport = outport;

    return cheapest;
}

// global_adaptive routing
void
fat_tree_global_adaptive::global_adaptive(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path & path) const
{
  // if path has not been selected, select one before pushing to outpor
  // assume current_sw_addr is the source switch
  if (!path.chosen.size()){
      const int ncal = nearest_common_ancestor_level(current_sw_addr, dest_sw_addr);

      // allocate space for path, starting with source
      path.chosen.clear();
      path.chosen.resize((ncal << 1) + 1);
      path.chosen.front() = routing_info::path::Hop(current_sw_addr, -1, 0);

      cheapest_path(0, 0, ncal, dest_sw_addr, path.chosen);
  }

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

}
} //end of namespace sstmac
