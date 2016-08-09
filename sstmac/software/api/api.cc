/*
 *  This file is part of SST/macroscale:
 *               The macroscale architecture simulator from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top
 *  SST/macroscale directory.
 */

#include <sstmac/software/process/operating_system.h>
#include <sstmac/software/api/api.h>
#include <sstmac/common/messages/sst_message.h>
#include <sstmac/common/sstmac_env.h>
#include <sprockit/sim_parameters.h>

ImplementFactory(sstmac::sw::api);

namespace sstmac {
namespace sw {

bool api::hostcompute_ = false;

void
api::init_param1(const software_id& sid)
{
  sid_ = sid;
}

void
api::init_factory_params(sprockit::sim_parameters* params)
{
  hostcompute_ = params->get_optional_bool_param("host_compute_modeling", false);
  if (hostcompute_) {
    timer_ = new Timer();
    compute_ = lib_compute_time::construct("api-hostcompute-" + sid_.to_string());
  }
}

void
api::start_api_call()
{
  if (hostcompute_) {
    if (endcount_ > 0
        && startcount_ == endcount_) { //can't do it on the first time through
      timer_->toc();

      timestamp t(timer_->getTime());
      compute_->compute(t);
    }
    startcount_++;
  }
}
void
api::end_api_call()
{
  if (hostcompute_) {
    timer_->tic();
    endcount_++;
  }
}

void
api::init_os(operating_system* os)
{
  library::init_os(os);

  if (hostcompute_) {
    register_lib(compute_);
  }
}

timestamp
api::now() const
{
  return os()->now();
}

void
api::schedule(timestamp t, event_queue_entry* ev)
{
  os()->schedule(t, ev);
}

void
api::schedule_delay(timestamp t, event_queue_entry* ev)
{
  os()->schedule_delay(t, ev);
}

Timer::Timer()
{

#if defined(_MAC)
  mach_timebase_info_data_t info;
  mach_timebase_info(&info);

  conv_factor = (static_cast<double> (info.numer))
                / (static_cast<double> (info.denom));
  conv_factor = conv_factor * 1.0e-9;

#else
  conv_factor = 1.0;
#endif

  reset();
}

inline void
Timer::tic()
{

#if defined(_MAC)
  start = mach_absolute_time();

#else
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  start = static_cast<double>(ts.tv_sec) + 1.0e-9 *
          static_cast<double>(ts.tv_nsec);

#endif
}

inline void
Timer::toc()
{
#if defined(_MAC)
  duration = static_cast<double> (mach_absolute_time() - start);

#else
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  duration = (static_cast<double>(ts.tv_sec) + 1.0e-9 *
              static_cast<double>(ts.tv_nsec)) - start;

#endif

  elapsed_time = duration * conv_factor;
}

void
Timer::reset()
{
  start = 0;
  duration = 0;
  elapsed_time = 0;
}
double
Timer::getTime()
{
  return elapsed_time;
}

}
}

