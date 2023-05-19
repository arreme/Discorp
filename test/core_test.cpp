#include <catch2/catch_test_macros.hpp>
#include <core/dcl_map.hpp>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/time_util.h>
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

TEST_CASE("Collecting a post", "[core][core_test_2]") 
{
    
    auto main_base = DCLData::DCLMap::getInstance().GetLocation(PBLocationID::MAIN_BASE);
    auto post_info = main_base->GetInteraction(0)->TryGetPost();
    PBUser user_db;
    PBPlayer *player = user_db.add_players();
    PBPostInteraction post_db;
    auto last_collected = post_db.mutable_last_collected();
    auto time = std::chrono::system_clock::now() - std::chrono::seconds(20);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
    last_collected->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));

    auto result = post_info->CalculatePostRewards(user_db.mutable_players(0),&post_db);
    REQUIRE(result.size() == 1);
    REQUIRE(result.at(0).item_id() == PBItemEnum::STICK);
    player->mutable_stats()->set_speed(10);
    auto result2 = post_info->CalculatePostRewards(user_db.mutable_players(0),&post_db);
    
    REQUIRE(result2.size() == 1);
    REQUIRE(result2.at(0).quantity() == 1);
}

