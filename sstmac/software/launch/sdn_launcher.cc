#include <sstmac/software/launch/sdn_launcher.h>

using namespace sstmac;
using namespace sstmac::sw;
using namespace sstmac::hw;

// SpktRegister("sdn", job_launcher, sdn_job_launcher);

// void
// sdn_job_launcher::handle_new_launch_request(int appnum, app_launch* appman)
// {
//   assert(interconnect_ != nullptr);
//   switch_interconnect* sw_ic = dynamic_cast<switch_interconnect*>(interconnect_);
//   auto& switches = sw_ic->switches();
//   for (network_switch* sw : switches){
//       auto pflow_sw = dynamic_cast<packet_flow_switch*>(sw);
//       auto stats = pflow_sw->xbar_stats();
//       //do whatever
//   }

// #if CUSTOM_LAUNCH
//   /** You can implement your own custom launch */
//   ordered_node_set allocation;
//   ordered_node_set& available = job_launcher::available_; //parent variable
//   appman->request_allocation(available, allocation);
//   //TODO check if allocation succeeded

//   //should always reuse this method at the end
//   job_launcher::satisfy_launch_request(appman);
// #else
//   /** Or you can directly use the default parent launch */
//   default_job_launcher::handle_new_launch_request(appman);
// #endif
// }

// void
// sdn_job_launcher::init_factory_params(sprockit::sim_parameters *params)
// {
//   //always call up to parent first
//   default_job_launcher::init_factory_params(params);
// }
