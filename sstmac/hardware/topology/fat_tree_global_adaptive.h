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
  virtual std::string
  to_string() const {
    return "fat tree topology (global adaptive)";
  }

  virtual ~fat_tree_global_adaptive() {}

  std::string
  default_router() const {
    return "fattree";
  }

  // sets the geometry_routable::path::chosen variable
  unsigned int
  cheapest_path(
    const std::size_t current_index,
    const std::size_t mid_point,
    const switch_id dst,
    std::vector <geometry_routable::path::Hop> & path) const;

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
