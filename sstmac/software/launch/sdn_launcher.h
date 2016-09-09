#include <sstmac/software/launch/job_launcher.h>
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sstmac/hardware/interconnect/switch_interconnect.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/router/router.h>
#include <cassert>

namespace sstmac {
namespace sw {

class sdn_job_launcher :
  public default_job_launcher
{
 public:
  virtual void
  handle_new_launch_request(int appnum, app_launch* appman);

  virtual void
  init_factory_params(sprockit::sim_parameters *params);
};

}
}
