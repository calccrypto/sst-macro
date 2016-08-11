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

#include <sstmac/hardware/topology/fat_tree_multipath.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_multipath", topology, fat_tree_multipath,
             "Fat tree topology with L levels, radix K, and multipath routing.");

// generate all possible paths to cycle through
void
fat_tree_multipath::all_paths(
    const std::size_t current_index,
    const std::size_t mid_point,
    const switch_id dst,
    Path & path,
    Paths & paths)
{
    // at the end of the path
    if ((current_index + 1) == path.size()){
      // save paths tha reach destination
      if (path[current_index].sw_id == dst){
        paths.second.push_back(path);
      }

      return;
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

      // try this current path
      all_paths(current_index + 1, mid_point, dst, path, paths);
    }
}

// multipath routing
// generates all paths and cycles through them
void
fat_tree_multipath::multipath(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path & path)
{
  // if path.src is not set, assume that the current switch is the source
  if (path.src == -1){
    path.src = current_sw_addr;
    path.dst = dest_sw_addr;
  }

  // get flow information
  Match_Fields mf;
  mf.src = path.src;
  mf.dst = path.dst;

  // find flow in flow table
  Flow_Table::iterator it = flow_table.find(mf);

  // if route not found, generate all possible routes and put them into the flow table
  if (it == flow_table.end()){
    const int ncal = nearest_common_ancestor_level(path.src, path.dst);

    // current working path
    Path temp;
    temp.resize((ncal << 1) + 1);
    temp[0] = geometry_routable::path::Hop(path.src, -1, 0);

    // all possible paths
    Paths paths;
    paths.first = 0;

    all_paths(0, ncal, path.dst, temp, paths);
    it = flow_table.insert(std::make_pair(mf, paths)).first;
  }

  // set the packet's route and move the counter to the next path
  path.chosen = it -> second.second[it -> second.first++];
  it -> second.first %= it -> second.second.size();

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
  //   spkt_throw_printf(sprockit::value_error, "fat_tree: multipath routing did not find route to next switch");
  // }

  top_debug("fat_tree: multipath routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_multipath::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path& path)
{
  multipath(current_sw_addr, dest_sw_addr, path);
}

}
} //end of namespace sstmac
