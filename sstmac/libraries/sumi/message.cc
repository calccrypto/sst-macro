#include <sstmac/libraries/sumi/message.h>
#include <sstmac/common/serializable.h>
#include <sumi/message.h>
#include <sprockit/util.h>
#include <iostream>

namespace sstmac {

transport_message::transport_message(sw::app_id aid,
 const sumi::message_ptr& msg, long byte_length)
  : library_interface("sumi"),
    network_message(aid, byte_length),
    payload_(msg)
{
}

void
transport_message::serialize_order(serializer& ser)
{
  network_message::serialize_order(ser);
  library_interface::serialize_order(ser);
  sumi::message* msg = payload_.get();
  ser & msg;
  payload_ = msg;
}

std::string
transport_message::to_string() const
{
  std::string message_str;
  sumi::message* smsg = ptr_test_cast(sumi::message, payload_);
  sstmac::message* msg = ptr_test_cast(sstmac::message, payload_);
  if (smsg){
    message_str = smsg->to_string();
  } else if (msg){
    message_str = msg->to_string();
  } else {
    message_str = "null payload";
  }
  return sprockit::printf("sumi transport message %lu carrying %s",
    unique_id(), message_str.c_str());
}

void
transport_message::put_on_wire()
{
  if (!is_metadata()){
    payload_->buffer_send();
  }
}

sstmac::hw::network_message*
transport_message::clone_injection_ack() const
{
#if SSTMAC_SANITY_CHECK
  if (network_message::type_ == network_message::null_netmsg_type){
    spkt_throw(sprockit::value_error,
        "message::clone_injection_ack: null network message type");
  }
#endif
  transport_message* cln = new transport_message;
  clone_into(cln);
#if SSTMAC_SANITY_CHECK
  if (cln->network_message::type() == network_message::null_netmsg_type){
    spkt_throw(sprockit::value_error,
        "message::clone_injection_ack: did not clone correctly");
  }
#endif
  cln->convert_to_ack();
  return cln;
}

void
transport_message::clone_into(transport_message* cln) const
{
  //the payload is actually immutable now - so this is safe
  cln->payload_ = payload_->clone();
  network_message::clone_into(cln);
  library_interface::clone_into(cln);
}

void
transport_message::reverse()
{
  //payload_->reverse();
  network_message::reverse();
}

}
