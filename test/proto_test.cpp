#include <catch2/catch_test_macros.hpp>
#include <core/pb/map.pb.h>
#include <google/protobuf/util/json_util.h>
#include <fstream>
#include <iostream>
#include <sstream>

TEST_CASE("Protobuf: Creating data","[proto][proto_test_1]") 
{   
    PBLocation test_location{};
    test_location.set_id(PBLocationID::GUILD_ENTRANCE_WEST);
    test_location.set_database_id(0);
    test_location.set_image("/resources");
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

TEST_CASE("Protobuf: Reading data","[proto][proto_test_2]") 
{
    std::ifstream t("test/data/test_2.json");
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto test_location = PBLocation{};
    auto status = google::protobuf::util::JsonStringToMessage(buffer.str(),&test_location);
    REQUIRE(status.ok());
    REQUIRE(test_location.id() == PBLocationID::GUILD_ENTRANCE_WEST);
    REQUIRE(test_location.database_id() == 0);
    REQUIRE(test_location.interactions(0).pos_x() == 20);
    REQUIRE(test_location.interactions(0).post_info().upgrades(0).info(0).current_stat() == 40.0f);

    std::ifstream t2("test/data/test_2_fail.json");
    std::stringstream buffer2;
    buffer2 << t.rdbuf();
    auto test_location2 = PBLocation{};
    auto status2 = google::protobuf::util::JsonStringToMessage(buffer2.str(),&test_location2);
    REQUIRE_FALSE(status2.ok());
}