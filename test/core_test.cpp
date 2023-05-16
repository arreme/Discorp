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
    auto image = main_base->GetLocationImage();
    REQUIRE(image == "resources/assets/Locations/0_main_base.png");
    auto interaction1 = main_base->GetInteraction(0);
    auto post1 = interaction1->TryGetPost(); 
    REQUIRE(post1);
    auto zone_access1 = interaction1->TryGetZoneAccess();
    REQUIRE_FALSE(zone_access1);
    
}

