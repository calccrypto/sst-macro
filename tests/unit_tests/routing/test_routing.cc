#include <tests/unit_tests/util/util.h>
#include <sprockit/output.h>

void test_torus(UnitTest& unit);
void test_crossbar(UnitTest& unit);
void test_fattree(UnitTest& unit);
void test_fattree_dmodk(UnitTest& unit);
void test_fattree_la(UnitTest& unit);
void test_fattree_ga(UnitTest& unit);
void test_butterfly(UnitTest& unit);
void test_fbfly(UnitTest& unit);
void test_dragonfly_v1(UnitTest& unit);
void test_dragonfly_v2(UnitTest& unit);

using namespace sstmac;

int main(int argc, char** argv)
{
    UnitTest unit;
    try{
        std::cout << "Testing torus...\n";
            test_torus(unit);
        std::cout << "Testing fat tree with default routing...\n";
            test_fattree(unit);
        std::cout << "Testing fat tree with DmodK routing...\n";
            test_fattree_dmodk(unit);
        std::cout << "Testing fat tree with local adaptive routing...\n";
            test_fattree_la(unit);
        std::cout << "Testing fat tree with global adaptive routing...\n";
            test_fattree_ga(unit);
        std::cout << "Testing crossbar...\n";
            test_crossbar(unit);
        std::cout << "Testing butterfly...\n";
            test_butterfly(unit);
        std::cout << "Testing fbfly...\n";
            test_fbfly(unit);
        std::cout << "Testing dragonfly...\n";
            test_dragonfly_v1(unit);
            test_dragonfly_v2(unit);
        unit.validate();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
