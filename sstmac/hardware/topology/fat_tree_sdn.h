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

#include <sstmac/hardware/topology/fat_tree_global_adaptive.h>
#include <sstmac/software/process/app_id.h>
#include <sstmac/software/process/flow_id.h>

namespace sstmac {
namespace hw {

/**
 * @class fat_tree with sdn routing
 * The fat tree network generates a k-ary fat tree with l tiers
 */
class fat_tree_sdn :
  public fat_tree_global_adaptive
{

 public:
  virtual std::string
  to_string() const {
    return "fat tree topology (sdn)";
  }

  virtual ~fat_tree_sdn() {}

  // current, each source destionation pair gets its own route
  void
  sdn(
    switch_id current_sw_addr,
    switch_id dest_sw_addr,
    geometry_routable::path &path);

  virtual void
  minimal_route_to_switch(
    switch_id current_sw_addr,
    switch_id dest_sw_addr,
    geometry_routable::path& path);

 private:
  // fields to match packets against
  struct Match_Fields {
//    sw::app_id app_id;
//    sw::flow_id flow_id;
    switch_id src;
    switch_id dst;

    Match_Fields(//const sw::app_id & appid = sw::app_id(),
                 //const sw::flow_id & flowid = sw::flow_id(),
                 const switch_id s = switch_id(),
                 const switch_id d = switch_id())
        : //app_id(appid), flow_id(flowid)
          src(s), dst(d)
        {}

    bool operator<(const Match_Fields & mf) const {
      return
//      (app_id < mf.app_id)?true:
//          ((flow_id < mf.flow_id)?true:
           ((src < mf.src)?true:
            (dst < mf.dst)
           )
//          )
            ;
    }
   };

  // map packet identifier to an output port
  typedef std::vector <geometry_routable::path::Hop> Path;
  typedef std::map <Match_Fields, Path> Flow_Table;
  Flow_Table flow_table;
};

}
} //end of namespace sstmac

#endif
