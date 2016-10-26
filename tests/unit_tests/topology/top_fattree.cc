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
    assertTrue(unit, "structured topology cast topology", bool(ftree));

    fat_tree * fattree = test_cast(fat_tree, ftree);
    assertTrue(unit, "fat tree cast structured topology", bool(fattree));

    /*
      Level 2: 32 (2, 0) - 47 (2, 15)
      Level 1: 16 (1, 0) - 31 (1, 15)
      Level 0:  0 (0, 0) - 15 (0, 15)
      -------------nodes-------------
    */

    // make sure switch_id -> coordinate and coordinate -> switch_id conversions work
    int id_good     = 0;
    int coords_good = 0;
    for(switch_id sw_id = 0; sw_id < 48; sw_id++){
        coordinates coords = fattree -> switch_coords(sw_id);
        switch_id test_id = fattree -> switch_number(coords);
        coordinates test_coords = fattree -> switch_coords(test_id);

        id_good     += (test_id == sw_id);
        coords_good += (test_coords == coords);
    }
    assertEqual(unit, "fattree switch id", id_good, 48);
    assertEqual(unit, "fattree coords", coords_good, 48);

    int match;
    coordinates coords(fattree -> ndimensions());

    // make sure level 0 up connections are correct
    match = 0;
    coords[0] = 0;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            match += ((16 + (coords[1] / 4) * 4 + port) ==
                      fattree -> switch_number(fattree -> neighbor_at_port(sw_id, 4 + port)));
        }
    }
    assertEqual(unit, "ftree level 0 up connection", match, 64);

    // make sure level 1 up connections are correct
    match = 0;
    coords[0] = 1;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            match += ((32 + (coords[1] % 4) + (4 * port)) ==
                      fattree -> switch_number(fattree -> neighbor_at_port(sw_id, 4 + port)));
        }
    }
    assertEqual(unit, "ftree level 1 up connection", match, 64);

    // make sure level 2 down connections are correct
    match = 0;
    coords[0] = 2;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            match += ((16 + (coords[1] % 4) + (4 * port)) ==
                      fattree -> switch_number(fattree -> neighbor_at_port(sw_id, port)));
        }
    }
    assertEqual(unit, "ftree level 2 down connection", match, 64);

    // make sure level 1 down connections are correct
    match = 0;
    coords[0] = 1;
    for(coords[1] = 0; coords[1] < 16; coords[1]++){
        // current switch id
        switch_id sw_id = fattree -> switch_number(coords);
        for(int port = 0; port < 4; port++){
            match += (((coords[1] / 4) * 4 + port) ==
                      fattree -> switch_number(fattree -> neighbor_at_port(sw_id, port)));
        }
    }
    assertEqual(unit, "ftree level 1 down connection", match, 64);
}