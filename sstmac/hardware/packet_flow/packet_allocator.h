#include <cassert>

#include <sstmac/hardware/network/network_message.h>
#include <sstmac/hardware/packet_flow/packet_allocator.h>
#include "sstmac/hardware/packet_flow/sdn_packet.h"

class sdn_packet_allocator : public sstmac::hw::packet_allocator
{
 public:
  virtual packet_flow_payload*
  new_packet(int bytes, long byte_offset, message *msg);

  virtual void
  init_factory_params(sprockit::sim_parameters *params);

};
