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

#ifndef SSTMAC_HARDWARE_PROCESSOR_EVENTDATA_H_INCLUDED
#define SSTMAC_HARDWARE_PROCESSOR_EVENTDATA_H_INCLUDED

#include <sstmac/common/messages/sst_message.h>
#include <sstmac/common/timestamp.h>
#include <sstmac/hardware/memory/memory_id.h>
#include <sprockit/debug.h>
#include <sprockit/typedefs.h>
#include <stdint.h>

DeclareDebugSlot(compute_intensity);

namespace sstmac {
namespace sw {

/**
 * Input for processor models that use
 * performance counter data. Is basically just a map
 * that maps std::string keys to integer values.
 * Keys are defined in the libraries that use them.
 */
class compute_event :
  public event,
  public serializable_type<compute_event>
{

  ImplementSerializableDefaultConstructor(compute_event)

 public:
  virtual void
  serialize_order(serializer &ser){}

  typedef enum {
    mem_random = 0,
    mem_sequential = 1,
    flop = 2,
    intop = 3,
    time = 4
  } event_type_t;

  compute_event();

  virtual ~compute_event() {}

  std::string
  to_string() const {
    return "compute message";
  }

  /**
   * @param key the key
   * @param val value to associate with the given key
   */
  void
  set_event_value(event_type_t ty, uint64_t val){
    event_data_[ty] = val;
  }

  /**
    * Get event data
    * @param key the key
    * @return value associated with given key
    */
  uint64_t
  event_value(event_type_t ty) const {
    return event_data_[ty];
  }

  void
  set_event_time(const timestamp& t) {
    event_data_[time] = t.ticks_int64();
  }

  std::string
  debug_string() const;

  timestamp
  event_time() const {
    return timestamp(event_data_[time], timestamp::exact);
  }

  bool
  timed_computed() const {
    return event_data_[time];
  }

  void
  set_core(int core){
    core_ = core;
  }

  int
  core() const {
    return core_;
  }

  hw::memory_access_id
  access_id() const {
    return unique_id_;
  }

  void
  set_access_id(hw::memory_access_id id) {
    unique_id_ = id;
  }

  uint64_t
  unique_id() const {
    return uint64_t(unique_id_);
  }

  long
  byte_length() const;

 private:
  int core_;

  #define MAX_EVENTS 8
  uint64_t event_data_[MAX_EVENTS];

  hw::memory_access_id unique_id_;

};

}
}  // end of namespace sstmac

#endif

