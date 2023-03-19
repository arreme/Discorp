#include <catch2/catch_test_macros.hpp>
#include <core/pb/map.pb.h>
#include <google/protobuf/util/json_util.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <core/dcl_map.hpp>

TEST_CASE("Protobuffers: Creating data","[proto]") 
{
    auto loc_data = PBLocation{};
    loc_data.set_locid(PBLocationID::MAIN_BASE);
    loc_data.set_locimage("assets/locations/main_base_0.png");
    auto inter_0 = loc_data.add_interactions();
    inter_0->set_type(PBInteractionType::POST);
    inter_0->set_posy(10);
    inter_0->set_posx(10);
    inter_0->set_interactionname("bushes");
    inter_0->set_resource(PBResourceItems::STICK);
    inter_0->add_imagepaths("assets/posts/bush_0.png");
    auto upgrade_0 = inter_0->add_upgradeinfo();
    upgrade_0->set_statname(PBUpgradeType::CAPACITY);
    auto upg_info_0 = upgrade_0->add_info();
    upg_info_0->set_currentstat(50);
    auto itemData_0 = upg_info_0->add_uprequirements();
    itemData_0->set_itemid(PBResourceItems::STICK);
    itemData_0->set_quantity(3);
    std::string output;
    google::protobuf::util::MessageToJsonString(loc_data,&output);
    std::ofstream out("resources/test.json");
    out << output;
}

TEST_CASE("Protobuffers: Reading data","[proto]") 
{
    std::ifstream t("resources/data/0_main_base.json");
    std::stringstream buffer;
    buffer << t.rdbuf();
    auto loc_data = PBLocation{};
    google::protobuf::util::JsonStringToMessage(buffer.str(),&loc_data);

    REQUIRE(loc_data.locid() == PBLocationID::MAIN_BASE);
    REQUIRE(loc_data.interactions()[0].databaseid() == 0);
    REQUIRE(loc_data.interactions()[0].resource() == PBResourceItems::STICK);
    REQUIRE(loc_data.interactions()[0].upgradeinfo()[0].statname() == PBUpgradeType::CAPACITY);
}

TEST_CASE("Game Map: Initializing the map","[map]") 
{
    auto location = GameMap::DCLMap::getInstance().GetLocation(PBLocationID::MAIN_BASE);
    REQUIRE(location->GetLocId() == PBLocationID::MAIN_BASE);
    
}

TEST_CASE("Getting Resources","[resources]") 
{
    auto location = GameMap::DCLMap::getInstance().GetLocation(PBLocationID::MAIN_BASE);
    REQUIRE(location->GetLocId() == PBLocationID::MAIN_BASE);
    PostInfo info{};
    Stats stat{};
    Skills skill{};
    info.ModifyLastCollected(-10);
    auto reward = location->CalculatePostRewards(0,&info,&stat,&skill);
    CHECK(reward[0].GetQuantity() == 2);
    REQUIRE(info.GetResourceStored() == 8);

    auto reward_2 = location->CalculatePostRewards(0,&info,&stat,&skill);
    CHECK(reward_2[0].GetQuantity() == 2);
    REQUIRE(info.GetResourceStored() == 6);
    REQUIRE(skill.m_forage_xp == 4);
    stat.m_speed = 15;
    info.ModifyLastCollected(-20);
    auto reward_3 = location->CalculatePostRewards(0,&info,&stat,&skill);
    std::cout << reward_3[0].GetQuantity() << std::endl;
    std::cout << info.GetResourceStored() << std::endl;
    std::cout << skill.m_forage_xp << std::endl;
    std::cout << skill.m_forage_lvl << std::endl;
}
