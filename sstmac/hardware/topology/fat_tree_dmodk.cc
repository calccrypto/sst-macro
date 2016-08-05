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

#include <sstmac/hardware/topology/fat_tree_dmodk.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_dmodk", topology, fat_tree_dmodk,
             "Fat tree topology with L levels, radix K, and DMODK routing.");

// dmodk routing
void
fat_tree_dmodk::dmodk(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path & path) const
{
  // get current switch coordinates
  coordinates curr(2);
  compute_switch_coords(current_sw_addr, curr);

  // get destination switch coordinates
  coordinates dst(2);
  compute_switch_coords(dest_sw_addr, dst);

  // if columns are the same
  if (path.vc = (curr[1] == dst[1])){
    path.outport = dst[1] % k_;          // go downwards [0, k_);
  }
  else{
    path.outport = (dst[1] % k_) + k_;   // go up and mod k towards destination column; up port range is [k, 2k)
  }

  top_debug("fat_tree: dmodk routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_dmodk::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  geometry_routable::path& path)
{
  dmodk(current_sw_addr, dest_sw_addr, path);
}

}
} //end of namespace sstmac
