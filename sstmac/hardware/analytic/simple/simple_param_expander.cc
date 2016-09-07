#include <sstmac/hardware/analytic/simple/simple_param_expander.h>
#include <sstmac/common/timestamp.h>
#include <sprockit/sim_parameters.h>

namespace sstmac {
namespace hw {

SpktRegister("simple", sstmac::param_expander, simple_param_expander);

void
simple_param_expander::expand(sprockit::sim_parameters* params)
{
  sprockit::sim_parameters* nic_params = params->get_optional_namespace("nic");
  sprockit::sim_parameters* node_params = params->get_optional_namespace("node");
  sprockit::sim_parameters* mem_params = node_params->get_optional_namespace("memory");
  sprockit::sim_parameters* switch_params = params->get_optional_namespace("switch");
  sprockit::sim_parameters* top_params = params->get_optional_namespace("topology");
  sprockit::sim_parameters* proc_params = node_params->get_optional_namespace("proc");

  nic_params->add_param_override("model", "simple");
  params->add_param_override("interconnect", "simple");
  switch_params->add_param_override("model", "simple");
  mem_params->add_param_override("model", "packet_flow");

  std::string amm_type = params->get_param("amm_model");
  if (amm_type == "amm1"){
    expand_amm1_memory(params, mem_params);
    expand_amm1_network(params, switch_params);
    expand_amm1_nic(params, nic_params, switch_params);
  }
  else if (amm_type == "amm2"){
    expand_amm2_memory(params, mem_params);
    expand_amm1_network(params, switch_params);
    expand_amm1_nic(params, nic_params, switch_params);
  }
  else if (amm_type == "amm3"){
    expand_amm2_memory(params, mem_params);
    expand_amm3_network(params, switch_params);
    expand_amm1_nic(params, nic_params, switch_params);
  }
  else if (amm_type == "amm4"){
    expand_amm4_nic(params, nic_params, switch_params);
  }
  else {
    spkt_throw_printf(sprockit::input_error, "invalid hardware model %s given",
        amm_type.c_str());
  }
}

void
simple_param_expander::expand_amm1_memory(
  sprockit::sim_parameters* params,
  sprockit::sim_parameters* mem_params)
{
  //now just get the strings
  std::string mem_bw_str = mem_params->get_param("bandwidth");
  mem_params->add_param_override("max_single_bandwidth", mem_bw_str);
  mem_params->add_param_override("total_bandwidth", mem_bw_str);
}

void
simple_param_expander::expand_amm1_network(
  sprockit::sim_parameters* params,
  sprockit::sim_parameters* switch_params)
{
  double link_bw = switch_params->get_bandwidth_param("link_bandwidth");
  double gbs = link_bw *param_expander::network_bandwidth_multiplier(params) / 1e9;
  std::string net_bw_str = sprockit::printf("%12.8fGB/s", gbs);
  switch_params->add_param_override("bandwidth", net_bw_str);
}

void
simple_param_expander::expand_amm1_nic(
 sprockit::sim_parameters* params,
 sprockit::sim_parameters* nic_params,
 sprockit::sim_parameters* switch_params)
{
  //nothing to do here
  std::string inj_bw_str = nic_params->get_param("injection_bandwidth");
  std::string inj_lat_str = nic_params->get_param("injection_latency");
  switch_params->add_param_override("injection_bandwidth", inj_bw_str);
  switch_params->add_param_override("injection_latency", inj_lat_str);
}

void
simple_param_expander::expand_amm2_memory(
 sprockit::sim_parameters* params,
 sprockit::sim_parameters* mem_params)
{
  expand_amm1_memory(params, mem_params);
  mem_params->add_param_override("max_single_bandwidth", params->get_param("max_memory_bandwidth"));
}

void
simple_param_expander::expand_amm3_network(
  sprockit::sim_parameters* params,
  sprockit::sim_parameters* switch_params)
{
  expand_amm1_network(params, switch_params);
  double link_bw = switch_params->get_bandwidth_param("link_bandwidth");
  double sw_multiplier = param_expander::switch_bandwidth_multiplier(params);
  double sw_bw = switch_params->get_bandwidth_param("crossbar_bandwidth") * sw_multiplier;
  //the network bandwidth is the min of link/sw bandwidth
  double net_bw = std::min(link_bw, sw_bw);
  double gbs = net_bw / 1e9;
  std::string net_bw_str = sprockit::printf("%12.8fGB/s", gbs);
  switch_params->add_param_override("bandwidth", net_bw_str);
}

void
simple_param_expander::expand_amm4_nic(
  sprockit::sim_parameters* params,
  sprockit::sim_parameters* nic_params,
  sprockit::sim_parameters* switch_params)
{
  spkt_throw(sprockit::unimplemented_error,
    "simple is not currently compatible with NIC model in abstract machine model amm4 -"
    "only a single injection pathway is used on the NIC, not distinct paths for RDMA and UDP sends");
}

}
}
