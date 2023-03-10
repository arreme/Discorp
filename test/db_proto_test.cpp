#include <catch2/catch_test_macros.hpp>
#include <core/pb/map.pb.h>
#include <google/protobuf/util/json_util.h>
#include <iostream>
#include <fstream>

TEST_CASE("Protobuffers: Creating data","[proto]") 
{
    auto loc_data = PBLocation{};
    loc_data.set_locid(PBLocationID::FOREST);
    loc_data.set_locimage("assets/locations/main_base_0.png");
    auto inter_0 = loc_data.add_interactions();
    inter_0->set_type(PBInteractionType::POST);
    inter_0->set_posy(10);
    inter_0->set_posx(10);
    inter_0->set_interactionname("bushes");
    inter_0->add_imagepaths("assets/posts/bush_0.png");
    auto upgrade_0 = inter_0->add_upgradeinfo();
    upgrade_0->set_statname(PBUpgradeType::CAPACITY);
    upgrade_0->set_currentlevel(0);
    upgrade_0->set_currentstat(50);
    auto itemData_0 = upgrade_0->add_requirements();
    itemData_0->set_itemid(PBItems::STICK);
    itemData_0->set_quantity(3);
    std::string output;
    google::protobuf::util::MessageToJsonString(loc_data,&output);
    std::ofstream out("resources/data/0_main_base.json");
    out << output;

    //std::string output = loc_data.DebugString();
    //std::ofstream out("resources/data/0_main_base.txt");
    //out << output;
    
}
