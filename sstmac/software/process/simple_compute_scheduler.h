#ifndef sstmac_software_process_simple_COMPUTE_scheduleR_H
#define sstmac_software_process_simple_COMPUTE_scheduleR_H

#include <sstmac/software/process/compute_scheduler.h>

namespace sstmac {
namespace sw {

class simple_compute_scheduler : public compute_scheduler
{
 public:
  simple_compute_scheduler() : ncore_active_(0) {}

  void reserve_core(thread* thr);

  void release_core(thread* thr);

  compute_scheduler*
  clone(operating_system* os) const {
    simple_compute_scheduler* cln = new simple_compute_scheduler;
    cln->os_ = os;
    compute_scheduler::clone_into(cln);
    return cln;
  }

 private:
  std::list<thread*> pending_threads_;
  int ncore_active_;
};

}
}

#endif // BASIC_COMPUTE_scheduleR_H
