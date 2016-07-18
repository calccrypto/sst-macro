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

#include <sstmac/hardware/topology/fat_tree_sdn.h>

namespace sstmac {
namespace hw {

SpktRegister("fattree_sdn", topology, fat_tree_sdn,
             "Fat tree topology with L levels, radix K, and packet/flow level SDN routing.");

void
fat_tree_sdn::sdn(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path & path) const
{
  // get outport from routing table; make sure the entries exist
  Routing_Table::const_iterator rt_it = routing_table.find(path.app_id);
  if (rt_it == routing_table.end()){
      return;
  }

  Flows::const_iterator fl_it = rt_it -> second.find(path.flow_id);
  if (fl_it == rt_it -> second.end()){
      return;
  }

  Switches::const_iterator sw_it = fl_it -> second.find(current_sw_addr);
  if (sw_it == fl_it -> second.end()){
      return;
  }

  // set outport to next hop
  path.outport = sw_it -> second;
  path.vc = (path.outport >= k_); // down ports are of the range [0, k)

  top_debug("fat_tree: sdn routing to get to %d from %d",
            int(dest_sw_addr),
            int(current_sw_addr));
}

void
fat_tree_sdn::minimal_route_to_switch(
  switch_id current_sw_addr,
  switch_id dest_sw_addr,
  routing_info::path& path) const
{
  // if route is not found, add entire route

  sdn(current_sw_addr, dest_sw_addr, path);
}

bool
fat_tree_sdn::add_sdn_paths(sdn_packet * pkt)
{
  if (!pkt){
      return false;
  }

  // if the flow is already installed, don't do anything
  if (routing_table[pkt -> app_id()][pkt -> flow_id()].size()){
      return false;
  }

  // have to convert node_id to switch_id first
  // although if each node has only 1 process, each node_id is the same as the switch_id
  switch_id src_sw = pkt -> fromaddr();
  switch_id dst_sw = pkt -> toaddr();

  // find nearest common ancestor level
  int level = 0;        // start at level 1
  int level_count = 1;  // level 1 has k switches per group
  while (src_sw != dst_sw){
      level_count *= k_;
      src_sw /= level_count;
      dst_sw /= level_count;
      level++;
  }

  // temporary variables
  int curr_cost = 0;
  std::vector <std::pair <switch_id, int> > curr_path;

  // final value
  int prev_cost = INT_MAX;
  std::vector <std::pair <switch_id, int> > cheapest;

  // find lowest cost path
  cheapest_path(src_sw, dst_sw, true, 0, level, curr_cost, curr_path, prev_cost, cheapest);

  // write chosen path into routing table
  for(std::pair <switch_id, int> const & sw_pt : cheapest){
      routing_table[pkt -> app_id()][pkt -> flow_id()][sw_pt.first] = sw_pt.second;
  }
  return true;
}

bool
fat_tree_sdn::cheapest_path(
    const switch_id & src,
    const switch_id & dst,

    const bool direction,                           // 0 for down, 1 for up
    const int current_level,
    const int nearest_common_level,

    int & curr_cost,
    std::vector <std::pair <switch_id, int> > & curr_path,

    int & prev_cost,
    std::vector <std::pair <switch_id, int> > & cheapest) const {

  // if done recursing, compare the paths and save the cheapest one
  if (src == dst){
    if (curr_cost < prev_cost){
      prev_cost = curr_cost;
      cheapest = std::move(curr_path);
      return true;
    }
  }

  // recurse through each possible path
}

bool
fat_tree_sdn::remove_sdn_paths(const sw::app_id & app_id){
    return routing_table.erase(app_id);
}

bool
fat_tree_sdn::remove_sdn_paths(const sw::app_id & app_id, const sw::flow_id & flow_id){
    routing_table[app_id].erase(flow_id);
}

}
} //end of namespace sstmac
