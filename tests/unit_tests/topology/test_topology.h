#include <sstmac/replacements/mpi.h>
#include <sstmac/util.h>
#include <sstmac/hardware/switch/network_switch.h>
#include <sstmac/hardware/network/network_message.h>
#include <sstmac/hardware/router/routable_message.h>
#include <sstmac/hardware/topology/topology.h>
#include <tests/unit_tests/util/util.h>
#include <sprockit/test/test.h>

void test_torus_traffic(UnitTest& unit);
void test_torus(UnitTest& unit);
void test_crossbar(UnitTest& unit);
void test_fattree2(UnitTest& unit);
void test_fattree4(UnitTest& unit);
void test_fattree_dmodk_2(UnitTest& unit);
void test_fattree_dmodk_4(UnitTest& unit);
void test_butterfly(UnitTest& unit);
void test_fbfly(UnitTest& unit);
void test_dragonfly_traffic(UnitTest& unit);
void test_dragonfly_v1(UnitTest& unit);
void test_dragonfly_v2(UnitTest& unit);
