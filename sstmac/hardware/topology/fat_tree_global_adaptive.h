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

#ifndef SSTMAC_HARDWARE_NETWORK_TOPOLOGY_FATTREE_GLOBAL_ADAPTIVE_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_TOPOLOGY_FATTREE_GLOBAL_ADAPTIVE_H_INCLUDED

#include <climits>
#include <list>
#include <map>

#include <sstmac/hardware/interconnect/switch_interconnect.h>
#include <sstmac/hardware/packet_flow/packet_flow_stats.h>
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/topology/fat_tree.h>

namespace sstmac {
namespace hw {

/**
 * @class fat_tree with global_adaptive routing
 * The fat tree network generates a k-ary fat tree with l tiers
 */
class fat_tree_global_adaptive :
  public fat_tree
{
 public:
  typedef std::vector <geometry_routable::path::Hop> Path;

  virtual std::string
  to_string() const {
    return "fat tree topology (global adaptive)";
  }

  virtual ~fat_tree_global_adaptive() {}

  std::string
  default_router() const {
    return "fattree";
  }

  void
  cheapest_path(
    const std::size_t current_index,
    const std::size_t midpoint,
    const switch_id dst,
    Path & current,                 // current path
    const std::size_t current_cost, // cost of current path
    Path & path,                    // final, chosen path
    std::size_t & path_cost) const; // cheapest path, initialized to INT_MAX

  // linear search on chosen path
  void
  global_adaptive(
    switch_id current_sw_addr,
    switch_id dest_sw_addr,
    geometry_routable::path &path) const;

  virtual void
  minimal_route_to_switch(
    switch_id current_sw_addr,
    switch_id dest_sw_addr,
    geometry_routable::path& path);
};

}
} //end of namespace sstmac

#endif
