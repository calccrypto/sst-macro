#include "test_topology.h"
#include <sstmac/hardware/topology/fat_tree.h>
#include <sstmac/hardware/router/fat_tree_router.h>
#include <sprockit/util.h>

using namespace sstmac;
using namespace sstmac::hw;

void
test_fattree(UnitTest& unit)
{
    sprockit::sim_parameters params;
    sstmac::env::params = &params;
    params["geometry"] = "3 4";
    topology* top = topology_factory::get_value("fattree", &params);

    structured_topology* ftree = test_cast(structured_topology, top);
    assertTrue(unit, "structured topology cast topology", bool(ftree) );

    fat_tree * fattree = test_cast(fat_tree, ftree);
    assertTrue(unit, "fat tree cast structured topology", bool(fattree) );

    /*
      Level 2: 32 (2, 0) - 47 (2, 15)
      Level 1: 16 (1, 0) - 31 (1, 15)
      Level 0:  0 (0, 0) - 15 (0, 15)
      -------------nodes-------------
    */

    // make sure switch_id -> coordinate and coordinate -> switch_id conversions work
    for(switch_id sw_id = 0; sw_id < 48; sw_id++){
        coordinates coords = fattree -> switch_coords(sw_id);
        switch_id test_id = fattree -> switch_number(coords);
        coordinates test_coords = fattree -> switch_coords(test_id);
        assertEqual(unit, "fattree switch id", test_id, sw_id);
        assertEqual(unit, "fattree coords", test_coords, coords);
    }

    coordinates coords(fattree -> ndimensions());

    // make sure level 0 up connections are correct
    coords[0] = 0;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            const switch_id math = 16 + (coords[1] / 4) * 4 + port;
            const switch_id calculated = fattree -> switch_number(fattree -> neighbor_at_port(sw_id, 4 + port));
            assertEqual(unit, "ftree level 0 up connection", calculated, math);
        }
    }

    // make sure level 1 up connections are correct
    coords[0] = 1;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            const switch_id math = 32 + (coords[1] % 4) + (4 * port);
            const switch_id calculated = fattree -> switch_number(fattree -> neighbor_at_port(sw_id, 4 + port));
            assertEqual(unit, "ftree level 1 up connection", calculated, math);
        }
    }

    // make sure level 2 down connections are correct
    coords[0] = 2;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            const switch_id math = 16 + (coords[1] % 4) + (4 * port);
            const switch_id calculated = fattree -> switch_number(fattree -> neighbor_at_port(sw_id, port));
            assertEqual(unit, "ftree level 2 down connection", calculated, math);
        }
    }

    // make sure level 1 down connections are correct
    coords[0] = 1;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            const switch_id math = (coords[1] / 4) * 4 + port;
            const switch_id calculated = fattree -> switch_number(fattree -> neighbor_at_port(sw_id, port));
            assertEqual(unit, "ftree level 1 down connection", calculated, math);
        }
    }
}

void
test_fattree2(UnitTest& unit)
{
    sprockit::sim_parameters params;
    sstmac::env::params = &params;
    params["geometry"] = "5 2";
    params["concentration"] = "2";
    topology* top = topology_factory::get_value("fattree", &params);
    structured_topology* ftree = test_cast(structured_topology, top);
    assertTrue(unit, "fat tree cast topology", bool(ftree) );

    switch_interconnect::switch_map switches;
    init_switches(switches, params, top);

    {
        long nids[] = {0, 13, 19, 37, 51, 66};
        for (int i=0; i < 6; ++i){
            switch_id nid(nids[i]);
            //make sure the functions work back and forth
            coordinates coords = ftree->switch_coords(nid);
            switch_id test_id = ftree->switch_number(coords);
            coordinates test_coords = ftree->switch_coords(test_id);
            assertEqual(unit, "ftree switch id", test_id, nid);
            assertEqual(unit, "ftree coords", test_coords, coords);
        }
    }

    /**
        Bottom Up
        Level 0: 0-15
        Level 1: 16-31
        Level 2: 32-47
        Level 3: 48-63
        Level 4: 64-71
        OR
        Top Down
        Level 0: 64-71
        Level 1: 48-63
        Level 2: 32-47
        Level 3: 16-31
        Level 4: 0-15
    */

    //lets test some coordinates for switches
    {
        coordinates coords = ftree->switch_coords(switch_id(69));
        assertEqual(unit, "fat tree coord check", coords, 0, 5);
    }

    //lets test some coordinates for switches
    {
        coordinates coords = ftree->switch_coords(switch_id(58));
        assertEqual(unit, "fat tree coord check", coords, 1, 1, 2);
    }

    //lets test some coordinates for switches
    {
        coordinates coords = ftree->switch_coords(switch_id(37));
        assertEqual(unit, "fat tree coord check", coords, 2, 0, 1, 1);
    }

    //lets test some coordinates for switches
    {
        coordinates coords = ftree->switch_coords(switch_id(30));
        assertEqual(unit, "fat tree coord check", coords, 3, 1, 1, 1, 0);
    }

    //lets test some coordinates for switches
    {
        coordinates coords = ftree->switch_coords(switch_id(13));
        assertEqual(unit, "fat tree coord check", coords, 4, 1, 1, 0, 1, 0);
    }

    {
    network_switch* sw = switches[switch_id(7)];
    router* router = sw->rter();

    geometry_routable::path path;
    packet_flow_payload* packet = msg(22);
    int port;
    router->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::up_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir",
    //               sw, path, 22);
    packet=msg(23);
    router->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    // this should rotate the paths taken
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::up_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir",
    //               sw, path, 23);
    packet = msg(15);
    router->route(packet);
    port = packet->next_port();
    assertEqual(unit, "route fat tree", port, 5);
    }


    {
    network_switch* sw = switches[switch_id(26)];
    router* router = sw->rter();

    geometry_routable::path path;
    packet_flow_payload* packet = msg(4);
    int port;
    router->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::up_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir",
    //                sw, path, 40);

    // this should rotate the paths taken
    packet = msg(4);
    router->route(packet);
    port = packet->next_port();
    ///path = packet->rinfo().current_path();
    //assert_dim_dir(unit, "fat tree dim/dir",
    //                sw, path, 42);

    packet = msg(15);
    router->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::up_dimension);

    packet = msg(23);
    router->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::down_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir",
    //                sw, path, 11);
    }


    {
    network_switch* sw = switches[switch_id(65)];
    router* rter = sw->rter();
    geometry_routable::path path;
    packet_flow_payload* packet = msg(4);
    int port;
    rter->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree from top", path.dim, (int) fat_tree::down_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir at top",
    //                sw, path, 50);
    packet = msg(15);
    rter->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::down_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir at top",
    //                sw, path, 51);
    packet = msg(23);
    rter->route(packet);
    port = packet->next_port();
    //path = packet->rinfo().current_path();
    //assertEqual(unit, "route fat tree", path.dim, (int) fat_tree::down_dimension);
    //assert_dim_dir(unit, "fat tree dim/dir at top",
    //                sw, path, 58);
    }


}
