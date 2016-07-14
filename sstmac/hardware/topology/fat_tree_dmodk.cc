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
#include <sstmac/hardware/router/router.h>
#include <sprockit/sim_parameters.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_dmodk", topology, fat_tree_dmodk,
             "Fat tree topology with L levels, radix K, and DMODK routing.");

// dmodk routing
void
fat_tree_dmodk::dmodk(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path & path) const
{
  // get current switch coordinates
  coordinates curr(2);
  compute_switch_coords(current_sw_addr, curr);

  // get destination switch coordinates
  coordinates dst(2);
  compute_switch_coords(dest_sw_addr, dst);

  path.vc = (curr[1] == dst[1]);                        // if columns are the same, go down; otherwise, go up
  path.outport = convert_to_port(path.vc, dst[1] % k_); // mod k towards destination column

  top_debug("fat_tree: dmodk routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_dmodk::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path& path) const
{
  dmodk(current_sw_addr, dest_sw_addr, path);
}

}
} //end of namespace sstmac
