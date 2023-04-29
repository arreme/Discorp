#include <catch2/catch_test_macros.hpp>
#include <core/pb/map.pb.h>
#include <google/protobuf/util/json_util.h>
#include <fstream>
#include <iostream>
#include <sstream>

TEST_CASE("Protobuf: Creating data","[proto][test_1]") 
{   
    PBLocation test_location{};
    test_location.set_loc_id(PBLocationID::MAIN_BASE);
    test_location.set_database_id(0);
    test_location.set_loc_image("/resources");
    auto int_1 = test_location.add_interactions();
    int_1->add_types(PBInteractionType::POST);
    int_1->set_pos_x(20);
    int_1->set_pos_y(10);
    auto post_info = int_1->mutable_post_info();
    auto post_info_upgrades = post_info->add_upgrades();
    post_info_upgrades->set_upgrade_type(PBUpgradeType::CAPACITY);
    auto post_info_upgrades_info = post_info_upgrades->add_info();
    post_info_upgrades_info->set_current_stat(40.0f);
    auto post_info_upgrades_info_upgrade_req = post_info_upgrades_info->add_upgrade_req();
    post_info_upgrades_info_upgrade_req->set_item_id(PBItemEnum::STICK);
    post_info_upgrades_info_upgrade_req->set_quantity(3);

    std::string output;
    google::protobuf::util::MessageToJsonString(test_location,&output);
    std::ofstream out("test/data/test_1.json");
    out << output;

    REQUIRE(true);
}

TEST_CASE("Protobuf: Reading data","[proto][test_2]") 
{
    std::ifstream t("test/data/test_2.json");
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto test_location = PBLocation{};
    google::protobuf::util::JsonStringToMessage(buffer.str(),&test_location);

    REQUIRE(test_location.loc_id() == PBLocationID::MAIN_BASE);
    REQUIRE(test_location.database_id() == 0);
    REQUIRE(test_location.interactions(0).pos_x() == 20);
    REQUIRE(test_location.interactions(0).post_info().upgrades(0).info(0).current_stat() == 40.0f);
}