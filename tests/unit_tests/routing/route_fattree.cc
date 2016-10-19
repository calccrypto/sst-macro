#include <tests/unit_tests/util/util.h>
#include <sstmac/hardware/topology/fat_tree.h>
#include <sstmac/hardware/router/fat_tree_router.h>
#include <sstmac/hardware/router/fat_tree_dmodk_router.h>
#include <sstmac/hardware/router/fat_tree_local_adaptive_router.h>
#include <sstmac/hardware/router/fat_tree_global_adaptive_router.h>
#include <sprockit/util.h>

using namespace sstmac;
using namespace sstmac::hw;

void
test_fattree(UnitTest& unit)
{
    sprockit::sim_parameters params;
    sstmac::env::params = &params;
    params["geometry"] = "3 4";
    params["radix"] = "4";
    params["num_levels"] = "3";
    params["router"] = "fattree";

    topology* top = topology_factory::get_value("fattree", &params);

    structured_topology* ftree = test_cast(structured_topology, top);
    assertTrue(unit, "fat tree from topology", bool(ftree));

    switch_interconnect::switch_map switches;
    init_switches(switches, params, top);

    {
        coordinates coords = get_vector(2,1);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();

        node_id dst = ftree->node_addr(get_vector(0,0,1));

        structured_routable::path_set paths;
        router->productive_paths_to_node(dst, paths);
        assertEqual(unit, "num productive ports", paths.size(), 1);
        assertEqual(unit, "productive port", paths[0].outport,
            top->convert_to_port(fat_tree::down_dimension, 0));
    }

    {
        coordinates coords = get_vector(0,11);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();

        node_id dst = ftree->node_addr(get_vector(0,3,1));

        structured_routable::path_set paths;
        router->productive_paths_to_node(dst, paths);
        assertEqual(unit, "num productive ports", paths.size(), 4);
        for (int k=0; k < paths.size(); ++k){
            assertEqual(unit, "productive port", paths[k].outport,
                        top->convert_to_port(fat_tree::up_dimension, k));
        }
    }

    {
        coordinates coords = get_vector(1,7);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();

        node_id dst = ftree->node_addr(get_vector(0,6),2);

        structured_routable::path_set paths;
        router->productive_paths_to_node(dst, paths);
        assertEqual(unit, "num productive ports", paths.size(), 1);
        assertEqual(unit, "productive port", paths[0].outport,
            top->convert_to_port(fat_tree::down_dimension, 2));
    }

    {
        coordinates c1 = get_vector(0,1);
        coordinates c2 = get_vector(0,11);
        int dist = ftree->minimal_distance(c1,c2);
        //I need to go up two steps
        int correct = 2 * 2;
        assertEqual(unit, "min distance", dist, correct);
    }

    {
        coordinates c1 = get_vector(0,1);
        coordinates c2 = get_vector(1,3);
        int dist = ftree->minimal_distance(c1,c2);
        //I need to go up one step
        int correct = 1;
        assertEqual(unit, "min distance", dist, correct);
    }
}

void
test_fattree_dmodk(UnitTest& unit)
{
    sprockit::sim_parameters params;
    sstmac::env::params = &params;
    params["geometry"] = "3 4";
    params["radix"] = "4";
    params["num_levels"] = "3";
    params["router"] = "fattree_dmodk";

    topology* top = topology_factory::get_value("fattree", &params);

    structured_topology* ftree = test_cast(structured_topology, top);
    assertTrue(unit, "structured topology from topology", (bool) ftree);

    fat_tree * fattree = test_cast(fat_tree, ftree);
    assertTrue(unit, "fat tree from structured topology", (bool) fattree);

    /*
      Level 2: 32 (2, 0) - 47 (2, 15)
      Level 1: 16 (1, 0) - 31 (1, 15)
      Level 0:  0 (0, 0) - 15 (0, 15)
      -------------nodes-------------
    */

    switch_interconnect::switch_map switches;
    init_switches(switches, params, top);
    structured_routable::path_set paths;

    // test number of possible paths (should be 1)
    {
        coordinates coords = get_vector(0, 0);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 3));

        // use router* to route
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "num productive ports for dmodk routing", paths.size(), 1);

        // get dmodk* from router*
        fat_tree_dmodk_router * dmodk = test_cast(fat_tree_dmodk_router, router);
        assertTrue(unit, "fat tree dmodk router from router", (bool) dmodk);

        // use dmodk* to route
        dmodk->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "num productive ports for dmodk routing", paths.size(), 1);
    }

    // level 0 -> 1
    // [(0, 0) -> (1, 3)] -> (0, 3)
    {
        coordinates coords = get_vector(0, 0);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 3));
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "(0, 0) -> (0, 3) at (0, 0) using port 7", paths[0].outport, top->convert_to_port(fat_tree::up_dimension, 3));
    }

    // level 1 -> 0
    // (0, 0) -> [(1, 3) -> (0, 3)]
    {
        coordinates coords = get_vector(1, 3);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 3));
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "(0, 0) -> (0, 3) at (1, 3) using port 3", paths[0].outport, top->convert_to_port(fat_tree::down_dimension, 3));
    }

    // level 0 -> 1
    // [(0, 0) -> (1, 3)] -> (2, 15) -> (1, 15) -> (0, 15)
    {
        coordinates coords = get_vector(0, 0);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 15));
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "(0, 0) -> (0, 15) at (0, 0) using port 7", paths[0].outport, top->convert_to_port(fat_tree::up_dimension, 3));
    }

    // level 1 -> 2
    // (0, 0) -> [(1, 3) -> (2, 15)] -> (1, 15) -> (0, 15)
    {
        coordinates coords = get_vector(1, 3);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 15));
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "(0, 0) -> (0, 15) at (1, 3) using port 7", paths[0].outport, top->convert_to_port(fat_tree::up_dimension, 3));
    }

    // level 2 -> 1
    // (0, 0) -> (1, 3) -> [(2, 15) -> (1, 15)] -> (0, 15)
    {
        coordinates coords = get_vector(2, 15);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 15));
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "(0, 0) -> (0, 15) at (2, 15) using port 3", paths[0].outport, top->convert_to_port(fat_tree::down_dimension, 3));
    }

    // level 1 -> 0
    // (0, 0) -> (1, 3) -> (2, 15) -> [(1, 15) -> (0, 15)]
    {
        coordinates coords = get_vector(1, 15);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 15));
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "(0, 0) -> (0, 15) at (1, 15) using port 3", paths[0].outport, top->convert_to_port(fat_tree::down_dimension, 3));
    }
}

void
test_fattree_la(UnitTest& unit)
{
    sprockit::sim_parameters params;
    sstmac::env::params = &params;
    params["geometry"] = "3 4";
    params["radix"] = "4";
    params["num_levels"] = "3";
    params["router"] = "fattree_la";

    topology* top = topology_factory::get_value("fattree", &params);

    structured_topology* ftree = test_cast(structured_topology, top);
    assertTrue(unit, "structured topology from topology", (bool) ftree);

    fat_tree * fattree = test_cast(fat_tree, ftree);
    assertTrue(unit, "fat tree from structured topology", (bool) fattree);

    /*
      Level 2: 32 (2, 0) - 47 (2, 15)
      Level 1: 16 (1, 0) - 31 (1, 15)
      Level 0:  0 (0, 0) - 15 (0, 15)
      -------------nodes-------------
    */

    switch_interconnect::switch_map switches;
    init_switches(switches, params, top);
    structured_routable::path_set paths;

    // test number of possible paths (should be 4)
    {
        coordinates coords = get_vector(0, 0);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 3));

        // use router* to route
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "num productive ports for local adaptive routing", paths.size(), 4);

        // get la* from router*
        fat_tree_local_adaptive_router * la = test_cast(fat_tree_local_adaptive_router, router);
        assertTrue(unit, "fat tree local adaptive router from router", (bool) la);

        // use la* to route
        la->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "num productive ports for local adaptive routing", paths.size(), 4);
    }
}

void
test_fattree_ga(UnitTest& unit)
{
    sprockit::sim_parameters params;
    sstmac::env::params = &params;
    params["geometry"] = "3 4";
    params["radix"] = "4";
    params["num_levels"] = "3";
    params["router"] = "fattree_ga";

    topology* top = topology_factory::get_value("fattree", &params);

    structured_topology* ftree = test_cast(structured_topology, top);
    assertTrue(unit, "structured topology from topology", (bool) ftree);

    fat_tree * fattree = test_cast(fat_tree, ftree);
    assertTrue(unit, "fat tree from structured topology", (bool) fattree);

    /*
      Level 2: 32 (2, 0) - 47 (2, 15)
      Level 1: 16 (1, 0) - 31 (1, 15)
      Level 0:  0 (0, 0) - 15 (0, 15)
      -------------nodes-------------
    */

    switch_interconnect::switch_map switches;
    init_switches(switches, params, top);
    structured_routable::path_set paths;

    // test number of possible paths (should be 4)
    {
        coordinates coords = get_vector(0, 0);
        switch_id swid = ftree->switch_number(coords);
        network_switch* sw = switches[swid];
        router* router = sw->rter();
        switch_id dst = ftree->switch_number(get_vector(0, 3));

        // use router* to route
        router->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "num productive ports for global adapive routing", paths.size(), 4);

        // get ga* from router*
        fat_tree_global_adaptive_router * ga = test_cast(fat_tree_global_adaptive_router, router);
        assertTrue(unit, "fat tree global adaptive router from router", (bool) ga);

        // use ga* to route
        ga->productive_paths_to_switch(dst, paths);
        assertEqual(unit, "num productive ports for global adaptive routing", paths.size(), 4);
    }
}