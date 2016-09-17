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

// hypercube.cc: Implementation of a high dimensional torus network.
//
// Author: Jeremiah Wilke <jjwilke@sandia.gov>
//

#include <sstmac/hardware/topology/hypercube.h>
#include <stdio.h>
#include <sstream>
#include <sprockit/stl_string.h>

namespace sstmac {
namespace hw {

SpktRegister("hypercube", topology, hypercube,
            "hypercube implements a high-dimension torus with an arbitrary number of dimensions");

hypercube::hypercube(sprockit::sim_parameters* params) :
  hdtorus(params)
{
  ndim_ = dimensions_.size();
  dim_to_outport_.resize(ndim_);
  int offset = 0;
  for (int i=0; i < ndim_; ++i) {
    dim_to_outport_[i] = offset;
    offset += dimensions_[i];
  }
  max_ports_intra_network_ = offset;
  eject_geometric_id_ = max_ports_intra_network_;
}

void
hypercube::minimal_route_to_switch(
  switch_id src,
  switch_id dst,
  routable::path& path) const
{
  int ndim = dimensions_.size();
  int div = 1;
  for (int i=0; i < ndim; ++i){
    int srcX = (src / div) % dimensions_[i];
    int dstX = (dst / div) % dimensions_[i];
    if (srcX != dstX){
      path.vc = 0;
      path.outport = convert_to_port(i, dstX);
      return;
    }
    div *= dimensions_[i];
  }
}

int
hypercube::minimal_distance(
  switch_id src,
  switch_id dst) const
{
  int dist = 0;
  int div = 1;
  int ndim = dimensions_.size();
  for (int i=0; i < ndim; ++i){
    int srcX = (src / div) % dimensions_[i];
    int dstX = (dst / div) % dimensions_[i];
    if (srcX != dstX){
      ++dist;
    }
    div *= dimensions_[i];
  }
  return dist;
}

void
hypercube::connect_objects(sprockit::sim_parameters* params, internal_connectable_map& objects)
{

  sprockit::sim_parameters* link_params = params->get_namespace("link");
  double bw = link_params->get_bandwidth_param("bandwidth");
  int bufsize = params->get_byte_length_param("buffer_size");
  for (int dim=0; dim < dimensions_.size(); ++dim){
    double port_bw = bw * red_[dim];
    int credits = bufsize * red_[dim];
    for (int dir=0; dir < dimensions_[dim]; ++dir){
      int port = convert_to_port(dim, dir);
      cartesian_topology::setup_port_params(port, credits, port_bw, link_params, params);
    }
  }

  top_debug("hypercube: connecting %d switches",
    int(objects.size()));

  for (auto& pair : objects) {
    switch_id myid(pair.first);
    connectable* me = pair.second;
    coordinates coords = switch_coords(myid);

    //loop every dimension and connect to all "neighbors"
    for (int dim=0; dim < dimensions_.size(); ++dim) {
      coordinates neighbor_coords = coords;
      switch_id my_id = switch_addr(coords);
      int dimsize = dimensions_[dim];
      int my_idx = coords[dim];
      int inport = convert_to_port(dim, my_idx);
      for (int dir=0; dir < dimsize; ++dir) {
        if (dir != my_idx) {
          neighbor_coords[dim] = dir;
          switch_id neighbor_id = switch_addr(neighbor_coords);
          connectable* neighbor_sw = objects[neighbor_id];
          top_debug("hypercube connecting %s(%d) to %s(%d) at dim=%d,dir=%d",
              stl_string(coords).c_str(), int(my_id),
              stl_string(neighbor_coords).c_str(), int(neighbor_id),
              dim, dir);

          int outport = convert_to_port(dim, dir);
          sprockit::sim_parameters* port_params = get_port_params(link_params, outport);

          me->connect_output(
            port_params,
            outport,
            inport,
            neighbor_sw);

          neighbor_sw->connect_input(
            port_params,
            outport,
            inport,
            me);

        }
      }
    }
  }
}


}
} //end of namespace sstmac




