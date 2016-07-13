#include <sstmac/software/launch/sdn_launcher.h>

using namespace sstmac;
using namespace sstmac::sw;
using namespace sstmac::hw;

// SpktRegister("sdn", sstmac::sw::job_launcher, sdn_job_launcher);

// void
// sdn_job_launcher::handle_new_launch_request(int appnum, app_launch* appman)
// {
//   switch_interconnect* swi = dynamic_cast<switch_interconnect*>(interconnect_);
//   assert(swi != nullptr);

//   for (const auto& pair : swi->switches()){
//     switch_id sid = pair.first;
//     network_switch* netsw = pair.second;
//     sdn_router* rtr = dynamic_cast<sdn_router*>(netsw->rter());
//     assert(rtr != nullptr);

//     //TODO assign your routing tables
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
