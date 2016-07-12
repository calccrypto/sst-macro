#include <sstmac/hardware/packet_flow/packet_flow_stats.h>
#include <sstmac/hardware/packet_flow/sdn_packet.h>

/**
 * @brief The sdn_stats class
 * There is one object per device (switch xbar, switch output buffer, nic)
 * You have to do some work to aggregate stats across devices
 */
//class sdn_stats : public sstmac::hw::packet_sent_stats
//{
//  public:
//   /**
//    * @brief collect_single_event
//    * Captures a single packet event at one of the crossbars or buffers
//    * This can collect stats about a single step in the network
//    * @param st
//    */
//   virtual void
//   collect_single_event(const sstmac::hw::packet_stats_st& st);

//   /**
//    * @brief collect_final_event
//    * Usually occurs on the NIC. When a packet has traversed its entire path,
//    * collect some stats about the entire network path
//    * @param pkt
//    */
//   virtual void
//   collect_final_event(packet_flow_payload *pkt);

//   /** Kind of ignore for now - we'll worry about this later */
//   virtual void
//   set_event_manager(event_manager *ev_mgr){}

//   virtual void
//   init_factory_params(sprockit::sim_parameters *params);
// };
