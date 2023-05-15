#include <catch2/catch_test_macros.hpp>
#include <core/dcl_map.hpp>
#include <google/protobuf/util/json_util.h>
#include <fstream>
#include <iostream>
#include <sstream>

TEST_CASE("Load Map Correctly from data folder","[core][core_test_1]") 
{
    auto main_base = DCLData::DCLMap::getInstance().GetLocation(PBLocationID::MAIN_BASE);
    REQUIRE(main_base);
}

