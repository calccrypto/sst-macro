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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREEDMODKROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREEDMODKROUTER_H_INCLUDED

#include <sstmac/hardware/router/fat_tree_router.h>

namespace sstmac {
namespace hw {

/**
 * @brief The fat_tree_dmodk_router class
 * Router encapsulating the special routing computations that must occur on
 * a fat tree topology.
 */
class fat_tree_dmodk_router :
  public fat_tree_router
{
 public:
  virtual ~fat_tree_dmodk_router();

  fat_tree_dmodk_router() :
      fat_tree_router()
  {
  }

  virtual void
  productive_paths_to_switch(
    switch_id dst,
    structured_routable::path_set &paths);

  virtual std::string
  to_string() const {
    return "fattreedmodkrouter";
  }

 protected:
  void
  minimal_route_to_switch(
    switch_id sw_addr,
    structured_routable::path& path);
};

}
}
#endif
