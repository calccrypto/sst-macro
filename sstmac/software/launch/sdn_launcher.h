#include <sstmac/software/launch/job_launcher.h>
#include <sstmac/software/launch/app_launch_fwd.h>
#include <sstmac/hardware/packet_flow/packet_flow_switch.h>
#include <sstmac/hardware/interconnect/switch_interconnect.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/router/router.h>
#include <cassert>
#include <vector>

namespace sstmac {
namespace sw {

class sdn_job_launcher :
  public job_launcher
{
 public:
  std::string
  to_string() const {
    return "sdn job launcher";
  }

  virtual void
  handle_new_launch_request(app_launch* appman);
};

}
}
