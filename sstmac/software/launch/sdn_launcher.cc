#include <sstmac/software/launch/sdn_launcher.h>
#include <sstmac/software/launch/app_launch.h>

namespace sstmac {
namespace sw {

SpktRegister("sdn", job_launcher, sdn_job_launcher);

void
sdn_job_launcher::handle_new_launch_request(app_launch* appman)
{
  assert(interconnect_ != nullptr);

  // get statistics of entire network
  hw::switch_interconnect * sw_ic = dynamic_cast<hw::switch_interconnect*>(interconnect_);
  const hw::macro_switch_interconnect::switch_map & switches = sw_ic->switches();
  for (auto sw : switches){
      auto pflow_sw = dynamic_cast<hw::packet_flow_switch*>(sw.second);
      auto stats = pflow_sw->xbar_stats();
      //do whatever
  }

  // generate routes based on the satistics and the topology
  for(std::pair <node_id, node_id> const & sd : std::vector <std::pair <node_id, node_id> > ()){
    for (std::pair <switch_id, int> const & hop : appman->topol()->cheapest_path(sd, nullptr)){
      // add this entry to the table (might have to change method for accessing the tables)
    }
  }


#if CUSTOM_LAUNCH
  /** You can implement your own custom launch */
  ordered_node_set allocation;
  appman->request_allocation(available_, allocation);
#else
  // copied from default_job_launcher::handle_new_launch_request
  ordered_node_set allocation;
  appman->request_allocation(available_, allocation);
  for (const node_id& nid : allocation){
    if (available_.find(nid) == available_.end()){
      spkt_throw_printf(sprockit::value_error,
                        "allocation requested node %d, but it's not available",
                        int(nid));
    }
    available_.erase(nid);
  }
  appman->index_allocation(allocation);
#endif

  satisfy_launch_request(appman);
}

}
}
