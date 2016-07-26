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

#ifndef SSTMAC_HARDWARE_NETWORK_ROUTING_ROUTING_INFO_H_INCLUDED
#define SSTMAC_HARDWARE_NETWORK_ROUTING_ROUTING_INFO_H_INCLUDED

#include <sprockit/metadata_bits.h>
#include <sprockit/unordered.h>
#include <sprockit/errors.h>
#include <sstmac/hardware/router/routing_enum.h>
#include <sstmac/common/node_address.h>
#include <sstmac/common/serializable.h>
#include <sstmac/software/process/app_id.h>
#include <sstmac/software/process/flow_id.h>
#include <vector>
#include <iostream>

namespace sstmac {
namespace hw {

class routing_info
{
 public:
  typedef enum {
    valiant_stage,
    final_stage,
    crossed_timeline,
  } metadata_slot;

  static const int uninitialized = -123;

  // one of these, if not both, should be static
  static sw::app_id APP_ID;
  static sw::flow_id FLOW_ID;

  struct path {
    int outport;
    int vc;
    /** An identifier indicating what geometric path on the topology this is following */
    int geometric_id;
    sprockit::metadata_bits<uint32_t> metadata;

//    // flow info
//    sw::app_id app_id;                                 // application id
//    sw::flow_id flow_id;                               // flow id within a job

    struct Hop {
        switch_id sw_id;                                 // at this switch
        int outport;                                     // go out this port
        int vc;                                          // in this direction

        Hop(switch_id sw = -1, int op = -1, int dir = -1)
            : sw_id(sw), outport(op), vc(dir)
            {}
    };

    std::vector <Hop> chosen;                            // chosen path

    path() :
      outport(uninitialized),
//      app_id(APP_ID++),
//      flow_id(FLOW_ID++),
#if SSTMAC_SANITY_CHECK
      vc(uninitialized)
#else
      vc(0)
#endif
    {
    }

    bool
    metadata_bit(metadata_slot slot) const {
      return metadata.bit(slot);
    }

    void
    set_metadata_bit(metadata_slot slot) {
      metadata.set_bit(slot);
    }

    void
    unset_metadata_bit(metadata_slot slot) {
      metadata.unset_bit(slot);
    }

    void
    clear_metadata() {
      metadata.clear();
    }

  };



  #define MAX_PATHS 32
  class path_set
  {
   public:
    path_set() : size_(0) {}
    int size() const { return size_; }
    void resize(int s){
      if (s > MAX_PATHS){
        spkt_throw_printf(sprockit::value_error,
          "routing_info::path_set size exceeds max %d", MAX_PATHS);
      }
      size_ = s;
    }

    path& operator[](int idx){
      return paths_[idx];
    }

   private:
    int size_;
    path paths_[MAX_PATHS];
  };

 public:
  routing_info() :
    algo_(routing::deflt)
  {
  }

  static const char*
  tostr(routing::algorithm_t algo);

  path&
  current_path() {
    return path_;
  }

  void
  assign_path(const path& pth) {
    path_ = pth;
  }

  void
  check_vc() {
    if (path_.vc == routing_info::uninitialized)
      path_.vc = 0;
  }

  virtual int
  vc() const {
    return path_.vc;
  }

  int
  port() const {
    return path_.outport;
  }

  routing::algorithm_t
  route_algo() const {
    return algo_;
  }

  void
  set_route_algo(routing::algorithm_t algo) {
    algo_ = algo;
  }

  void
  init_default_algo(routing::algorithm_t algo){
    algo_ = routing::deflt ? algo : algo_;
  }

  void
  set_dest_switch(switch_id sid) {
    dest_switch_ = sid;
  }

  switch_id
  dest_switch() const {
    return dest_switch_;
  }

 protected:
  path path_;
  switch_id dest_switch_;
  routing::algorithm_t algo_;

};

}
}

START_SERIALIZATION_NAMESPACE
template <>
class serialize<sstmac::hw::routing_info>
{
 public:
  void
  operator()(sstmac::hw::routing_info& info, serializer& ser){
    ser.primitive(info);
  }
};

template <>
class serialize<sstmac::hw::routing_info::path>
{
 public:
  void
  operator()(sstmac::hw::routing_info::path& info, serializer& ser){
    ser.primitive(info);
  }
};
END_SERIALIZATION_NAMESPACE

#endif
