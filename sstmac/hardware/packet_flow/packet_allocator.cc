#include "sstmac/hardware/packet_flow/packet_allocator.h"

using namespace sstmac;
using namespace sstmac::sw;
using namespace sstmac::hw;

SpktRegister("sdn", sstmac::hw::packet_allocator, sdn_packet_allocator);

packet_flow_payload*
sdn_packet_allocator::new_packet(int bytes, long byte_offset, message *msg)
{
  //if you need more app data, we can dicuss ways to get more information
  //about the message (flow) being sent
  network_message* netmsg = dynamic_cast<network_message*>(msg);
  assert(netmsg != nullptr);

  //the app id can now be tacked onto packets
  app_id aid = netmsg->aid();

  //you can use the ctor I have - or make your own
  return new sdn_packet(netmsg, bytes, byte_offset);
}

void
sdn_packet_allocator::init_factory_params(sprockit::sim_parameters *params)
{
  //always call up to parent
  packet_allocator::init_factory_params(params);
  //TODO read in params
}
