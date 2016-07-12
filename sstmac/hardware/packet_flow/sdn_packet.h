#ifndef PACKET_H
#define PACKET_H

#include <sstmac/hardware/packet_flow/packet_flow.h>
#include <sstmac/software/process/app_id.h>
#include <sstmac/hardware/network/network_message.h>

/**
 * @brief The sdn_packet class
 * Add on whatever metadata you want to the original packet
 * Inside your router you'll cast packets to this type
 */
class sdn_packet : public sstmac::hw::packet_flow_payload
{
  //we'll worry about serialization and MPI parallel later
  NotSerializable(sdn_packet)
 public:
  /**
   * @brief sdn_packet
   * @param parent  Takes a specific network_message type
   * @param num_bytes
   * @param offset
   */
  sdn_packet(sstmac::hw::network_message* parent, int num_bytes, long offset) :
    aid_(parent->app_id()),
    toaddr_(parent->toaddr()),
    fromaddr_(parent->fromaddr()),
    packet_flow_payload(parent, num_bytes, offset)
  {
    //you grab the app id for use in routing later

    //CAUTION - only the tail packet will actually carry the parent payload
  }

  //these methods are required as part of the packet flow interface
  //your routing scheme MUST tell the packet flow switch what path to follow
  virtual int next_port() const {
    return -1; //TODO
  }
  virtual int next_vc() const {
    return -1; //TODO
  }

  virtual sstmac::node_id
  toaddr() const {
    return toaddr_;
  }
  virtual sstmac::node_id
  fromaddr() const {
    return fromaddr_;
  }

  sstmac::sw::app_id
  aid() const {
    return aid_;
  }

 protected:
  /** Add extra routing metadata */
  sstmac::sw::app_id aid_;
  sstmac::node_id toaddr_;
  sstmac::node_id fromaddr_;

  /** Add extra stats metadata */

};

#endif // PACKET_H
