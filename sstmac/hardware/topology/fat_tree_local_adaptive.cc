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

#include <climits>

#include <sstmac/hardware/topology/fat_tree_local_adaptive.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_local_adaptive", topology, fat_tree_local_adaptive,
             "Fat tree topology with L levels, radix K, and multipath adaptive routing.");

void
fat_tree_local_adaptive::adaptive(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path & path)
{
  coordinates dest_coords;
  compute_switch_coords(dest_sw_addr, dest_coords);

  // get highest level necessary
  const int ncal = nearest_common_ancestor_level(current_sw_addr, dest_coords);

  path.outport = -1; // start with no port

  // if going down (current switch is in the same level as the nearest common ancestor)
  if (path.vc = (ncal == dest_coords[0])){
    for(int p = 0; p < k_; p++){
      const coordinates below = neighbor_at_port(current_sw_addr, p);
      if ((below == dest_coords) ||                                               // the switch is the destination
          ((ncal - 1) == nearest_common_ancestor_level(below, dest_sw_addr))){    // the switch and the destination have the same ancestor (are in the same branch)
        // if (clone_map[p].get_stat() < clone_map[path.outport].get_stat()){
        //   path.outport = p;
        // }
      }
    }
  }
  else{
    for(int p = k_; p < (2 * k_); p++){
      const coordinates above = neighbor_at_port(current_sw_addr, p);
      if ((ncal - 1) == nearest_common_ancestor_level(above, dest_sw_addr)){     // the switch and the destination have the same ancestor (are in the same branch)
        // if (clone_map[p].get_stat() < clone_map[path.outport].get_stat()){
        //     path.outport = p;
        // }
      }
    }
  }

  top_debug("fat_tree: adaptive routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_local_adaptive::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path& path) const
{
//  adaptive(current_sw_addr, dest_sw_addr, path);
}


}
} //end of namespace sstmac
