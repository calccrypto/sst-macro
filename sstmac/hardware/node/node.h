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

#ifndef SSTMAC_BACKENDS_NATIVE_COMPONENTS_NODE_NODE_H_INCLUDED
#define SSTMAC_BACKENDS_NATIVE_COMPONENTS_NODE_NODE_H_INCLUDED

#include <sstmac/software/ami/ami.h>
#include <sstmac/common/timestamp.h>
#include <sstmac/common/event_scheduler.h>
#include <sstmac/common/sst_event_fwd.h>
#include <sstmac/hardware/common/unique_id.h>
#include <sstmac/hardware/common/failable.h>

#include <sstmac/hardware/topology/topology_fwd.h>
#include <sstmac/hardware/network/network_message_fwd.h>
#include <sstmac/software/launch/app_launch_fwd.h>
#include <sstmac/software/process/operating_system_fwd.h>
#include <sstmac/software/launch/launcher_fwd.h>
#include <sstmac/software/launch/launch_event_fwd.h>
#include <sstmac/software/libraries/service_fwd.h>
#include <sstmac/hardware/nic/nic_fwd.h>
#include <sstmac/hardware/memory/memory_model_fwd.h>
#include <sstmac/hardware/processor/processor_fwd.h>

#include <sprockit/factories/factory.h>
#include <sprockit/debug.h>

DeclareDebugSlot(node);
#define node_debug(...) \
  debug_printf(sprockit::dbg::node, "Node %d: %s", int(addr()), sprockit::printf(__VA_ARGS__).c_str())

namespace sstmac {
namespace hw {

class node :
  public sprockit::factory_type,
  public failable,
  public connectable_component
{

 public:
#if SSTMAC_INTEGRATED_SST_CORE
  node(
    SST::ComponentId_t id,
    SST::Params& params
  );

  void setup();

  void init(unsigned int phase);
#endif
  virtual void
  set_event_manager(event_manager* man);


  virtual ~node();
  /**
   Standard factory type initializer. Perform extra initialization work
   after all parameters have been read in.
   */
  virtual void
  finalize_init();

  /**
   Standard factory type initializer. Read in all parameters.
   @params The parameter object
   */
  virtual void
  init_factory_params(sprockit::sim_parameters* params);

  /**
   Initializer used in stand-alone core.
   @param n  The network interface object
  */
  void
  set_nic(nic* n){
    nic_ = n;
  }

  void
  connect(int src_outport, int dst_inport,
    connection_type_t ty, connectable *mod,
    config *cfg);

  /**
   @return  The object encapsulating the memory model
  */
  memory_model*
  mem() const {
    return mem_model_;
  }

  /**
   @return  A handler wrapper for scheduling events to the NIC
  */
  nic*
  get_nic() const {
    return nic_;
  }

  /**
   @return  The operating system managing apps on this node
  */
  sw::operating_system*
  os() const {
    return os_;
  }

  /**
   @return  A unique string description of the node
  */
  virtual std::string
  to_string() const;

  /**
   @return  A unique integer identifier
  */
  node_id
  addr() const {
    return my_addr_;
  }

  /**
   Cause the node to crash. This cancels all future events for this node.
  */
  void fail_stop();

  /**
   Choose a unique (64-bit) integer ID for a message. This will never be reused
   except for integer overflow.
   @return A unique 64-bit integer
  */
  unique_event_id
  allocate_unique_id() {
    return next_outgoing_id_++;
  }

  /**
   Make the node execute a particular compute function. This
   generally causes the function to be executed immediately.
   @param func  Enum identifying the type of computation
   @param data  Event object encapsulating data/metadata for computation
   @param cb    The event to execute when kernel is complete
  */
  virtual void
  execute(ami::COMP_FUNC func,
           event* data,
           callback* cb) = 0;

  /**
   * @brief execute Asynchronously execute a kernel on some
   * service associated with the node. This generally enqueues an operation
   * to be performed - not necessarily executing it immediately.
   * @param func
   * @param data
   */
  virtual void
  execute(ami::SERVICE_FUNC func,
                 event* data);

  virtual void
  handle(event* ev);

  /**
   Push a network message (operation at the MTL layer) onto the NIC
   @param netmsg
  */
  void send_to_nic(network_message* netmsg);

 protected:
  node();

  void connect_nic();

 protected:
  sw::app_launch* env_;

  sw::operating_system* os_;

  node_id my_addr_;

  memory_model* mem_model_;

  processor* proc_;

  nic* nic_;

  int ncores_;

  int nsocket_;

 private:
  void build_launchers(sprockit::sim_parameters* params);

 private:
  std::list<sw::launch_event*> launchers_;
  unique_event_id next_outgoing_id_;

#if !SSTMAC_INTEGRATED_SST_CORE
 public:
  void launch(timestamp start, sw::launch_event* msg);
#else
  void launch();
#endif

};

DeclareFactory(node);

}
} // end of namespace sstmac

#endif

