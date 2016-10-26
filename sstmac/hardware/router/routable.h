#ifndef sstmac_hardware_network_interconnect_INTERCONNECT_MESSAGE_H
#define sstmac_hardware_network_interconnect_INTERCONNECT_MESSAGE_H

#include <sstmac/common/messages/sst_message.h>
#include <sstmac/hardware/router/routing_enum.h>

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

   path() :
     outport(routing::uninitialized),
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

  typedef std::vector <path> path_set;

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

