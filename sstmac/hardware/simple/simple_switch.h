#ifndef SIMPLE_SWITCH_H
#define SIMPLE_SWITCH_H

#include <sstmac/common/event_handler.h>
#include <sstmac/common/event_scheduler.h>
#include <sstmac/common/messages/sst_message_fwd.h>
#include <sstmac/hardware/nic/nic_fwd.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/interconnect/interconnect_fwd.h>
#include <sprockit/unordered.h>

namespace sstmac {
namespace hw {

class simple_switch :
  public network_switch
{

 public:
  simple_switch(sprockit::sim_parameters* params, uint64_t id, event_manager* mgr);

  /**
   Cast message and pass to #send
   @param msg Incoming message (should cast to packet_train)
   */
  virtual void
  handle(event* ev) override;

  virtual std::string
  to_string() const override {
    return "simple switch";
  }

  int queue_length(int port) const override {
    return 0;
  }

  virtual
  ~simple_switch();

  void
  add_switch(event_handler* netsw, node_id nid);

  void
  add_nic(event_handler* theNic, node_id nid);

  void connect_output(sprockit::sim_parameters *params,
                      int src_outport, int dst_inport,
                      connectable *mod) override;

  void connect_input(sprockit::sim_parameters *params,
                     int src_outport, int dst_inport,
                     connectable *mod) override;

 private:
  double inj_bw_inverse_;

  timestamp inj_lat_;

  double inverse_bw_;

  double inv_min_bw_;

  timestamp hop_latency_;

  interconnect* interconn_;

  std::vector<event_handler*> neighbors_;
  std::vector<event_handler*> nics_;

};

}
}

#endif // SIMPLE_SWITCH_H
