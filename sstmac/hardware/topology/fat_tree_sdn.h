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

#ifndef SSTMAC_HARDWARE_NETWORK_TOPOLOGY_FATTREE_SDN_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_TOPOLOGY_FATTREE_SDN_H_INCLUDED

#include <map>

#include <sstmac/hardware/packet_flow/sdn_packet.h>
#include <sstmac/hardware/topology/fat_tree_dmodk.h>
#include <sstmac/software/process/app_id.h>
#include <sstmac/software/process/flow_id.h>

namespace sstmac {
namespace hw {

/**
 * @class fat_tree with SDN routing
 * The fat tree network generates a k-ary fat tree with l tiers
 */
class fat_tree_sdn :
  public fat_tree_dmodk
{

 public:
  virtual std::string
  to_string() const {
    return "fat tree topology (sdn)";
  }

  virtual ~fat_tree_sdn() {}

  std::string
  default_router() const {
    return "fattree (sdn)";
  }

  void
  sdn(switch_id current_sw_addr,
      switch_id dest_sw_addr,
      routing_info::path &path) const;

  virtual void
  minimal_route_to_switch(
      switch_id current_sw_addr,
      switch_id dest_sw_addr,
      routing_info::path& path) const;

 private:
    // routing_table[app_id][flow_id][current_switch] -> outport
    typedef std::map <switch_id, int> Switches;
    typedef std::map <sw::flow_id, Switches> Flows;
    typedef std::map <sw::app_id, Flows> Routing_Table;
    Routing_Table routing_table;

    // get the cheapest path and write it into the routing table
    bool
    add_sdn_paths(sdn_packet * pkt);

    // scan all possible paths from src to dst and save the cheapest path
    bool
    cheapest_path(
        const switch_id & src,
        const switch_id & dst,

        const bool direction,                           // 0 for down, 1 for up
        const int current_level,
        const int nearest_common_level,

        int & curr_cost,
        std::vector <std::pair <switch_id, int> > & curr_path,

        int & prev_cost,
        std::vector <std::pair <switch_id, int> > & cheapest) const;

    bool
    remove_sdn_paths(const sw::app_id & app_id);

    bool
    remove_sdn_paths(const sw::app_id & app_id, const sw::flow_id & flow_id);
};


}
} //end of namespace sstmac

#endif
