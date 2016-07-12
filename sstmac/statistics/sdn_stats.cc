#include <sstmac/statistics/sdn_stats.h>
#include <cassert>

using namespace sstmac;
using namespace sstmac::sw;
using namespace sstmac::hw;

// SpktRegister("sdn", sstmac::hw::packet_sent_stats, sdn_stats);

// /** Reproduced here
// struct packet_stats_st
// {
//   double incoming_bw;
//   double outgoing_bw;
//   timestamp now;
//   timestamp head_leaves;
//   timestamp tail_leaves;
//   timestamp credit_leaves;
//   packet_flow_payload* pkt;
//   int src_outport;
//   int dst_inport;
// };
// */

// void
// sdn_stats::collect_single_event(const packet_stats_st &st)
// {
//   int myid = id(); //either a switch or node id, depending on device
//   double outgoing_bw = st.pkt->bw();
//   //time (in sec) when the packet arrived at the current device
//   double arrival = st.pkt->arrival();

//   sdn_packet* pkt = dynamic_cast<sdn_packet*>(st.pkt);
//   assert(pkt != nullptr);
//   //TODO add any stats you want to the pkt for use in collect_final_event
// }

// void
// sdn_stats::collect_final_event(packet_flow_payload* pkt_)
// {
//   sdn_packet* pkt = dynamic_cast<sdn_packet*>(pkt_);
//   assert(pkt != nullptr);
//   //do any stats collection on the receiving NIC that you want
// }

// void
// sdn_stats::init_factory_params(sprockit::sim_parameters *params)
// {
//   //always call up to parent first
//   packet_sent_stats::init_factory_params(params);
// }
