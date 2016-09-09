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

#ifndef SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREEGLOBALADAPTIVEROUTER_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_SWTICHES_ROUTING_FATTREEGLOBALADAPTIVEROUTER_H_INCLUDED

#include <sstmac/hardware/router/fat_tree_router.h>
#include <set>

namespace sstmac {
namespace hw {

/**
 * @brief The fat_tree_global_adaptive_router class
 * Router encapsulating the special routing computations that must occur on
 * a fat tree topology.
 */
class fat_tree_global_adaptive_router :
  public fat_tree_router
{
 public:
  virtual ~fat_tree_global_adaptive_router();

  fat_tree_global_adaptive_router() :
      fat_tree_router()
  {
  }

  virtual std::string
  to_string() const {
    return "fattreegarouter";
  }

  // fields used to distinguish between packets
  struct Match_Fields {
    // app id
    // flow id
    switch_id src;
    switch_id dst;
    bool operator==(const Match_Fields & mf) const {
      return ((src == mf.src) &&
              (dst == mf.dst));
    }

    bool operator<(const Match_Fields & mf) const {
      return ((src < mf.src)?true:
              (dst < mf.dst));
    }

  };

  // (src, dst) -> outport
  struct Entry {
      Match_Fields match_fields;
      int vc;
      int outport;

      bool operator==(const Entry & entry) const {
          return ((match_fields == entry.match_fields) && (vc == entry.vc) && (outport == entry.outport));
      }

      bool operator<(const Entry & entry) const {
          return ((match_fields < entry.match_fields)?true:((vc < entry.vc)?true:(outport < entry.outport)));
      }
  };

  typedef std::set <Entry> Table;

  void
  add_entry(const Entry & entry);

  // no delete entry

  Match_Fields *
  get_packet_metadata(packet * pkt) const;

  virtual void
  route(packet* pkt);

 private:
  Table table;
};

}
}
#endif
