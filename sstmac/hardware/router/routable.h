#ifndef sstmac_hardware_network_interconnect_INTERCONNECT_MESSAGE_H
#define sstmac_hardware_network_interconnect_INTERCONNECT_MESSAGE_H

#include <sstmac/common/messages/sst_message.h>
#include <sstmac/hardware/router/routing_enum.h>
#include <vector>

namespace sstmac {
namespace hw {

class geometry_routable
{
 public:
  typedef enum {
   valiant_stage,
   final_stage,
   crossed_timeline
  } metadata_slot;

  struct path {
   int outport;
   int vc;
   /** An identifier indicating what geometric path on the topology this is following */
   int geometric_id;
   sprockit::metadata_bits<uint32_t> metadata;

   struct Hop {
     switch_id sw_id;                                 // at this switch
     int outport;                                     // go out this port
     int vc;                                          // in this direction

     Hop(switch_id sw = -1, int op = -1, int dir = -1)
         : sw_id(sw), outport(op), vc(dir)
         {}
   };

   std::vector <Hop> chosen;                          // full path for when doing source routing
   switch_id src;                                     // source switch
   switch_id dst;                                     // destionation switch
   int id;

   path() :
     outport(routing::uninitialized),
     src(-1),
     dst(-1),
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
         "routable::path_set size exceeds max %d", MAX_PATHS);
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
  node_id
  toaddr() const {
    return toaddr_;
  }

  node_id
  fromaddr() const {
    return fromaddr_;
  }

  void
  set_toaddr(node_id to) {
    toaddr_ = to;
  }

  void
  set_fromaddr(node_id from) {
    fromaddr_ = from;
  }

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
    if (path_.vc == routing::uninitialized)
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
  void
  serialize_order(serializer& ser);


  void
  set_dest_switch(switch_id sid) {
    dest_switch_ = sid;
  }

  switch_id
  dest_switch() const {
    return dest_switch_;
  }

 protected:
  geometry_routable() {}

  geometry_routable(node_id toaddr, node_id fromaddr);

 private:
  node_id toaddr_;

  node_id fromaddr_;

  path path_;

  switch_id dest_switch_;

};

}
}

START_SERIALIZATION_NAMESPACE
template <>
class serialize<sstmac::hw::geometry_routable::path>
{
 public:
  void
  operator()(sstmac::hw::geometry_routable::path& info, serializer& ser){
    ser.primitive(info);
  }
};
END_SERIALIZATION_NAMESPACE

#endif // INTERCONNECT_MESSAGE_H
