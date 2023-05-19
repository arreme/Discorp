#include <catch2/catch_test_macros.hpp>
#include <core/pb/player.pb.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>
#include <db_handler/db_inventory.hpp>

TEST_CASE("User Database Handler Testing","[db_handler][db_handler_test_1]") 
{   
    db::DeleteManyOperation del_op_usrs = db::DeleteManyOperation{"users", make_document()};
    del_op_usrs.ExecuteOperation();
    PBUser user;
    user.set_discord_id(0);
    user.set_user_name("Arreme");
    user.set_current_player_id(0);
    auto last_online = user.mutable_last_online();
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    last_online->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
    auto player = user.add_players();
    player->set_version("0.0.1");
    player->set_guild_id(10);
    player->set_current_location(PBLocationID::MAIN_BASE);
    {
        auto equip = player->mutable_equipation();
        auto skills = player->mutable_skills();
        skills->set_foraging_lvl(1);
        equip->add_attacks(PBItemEnum::KICK);
    }
    db_handler::DBUserHandler db_user_handler{&user};
    REQUIRE(db_user_handler.InsertUser());
    player->set_guild_id(230);
    REQUIRE(db_user_handler.FindUserCurrentPlayer());
    REQUIRE(player->guild_id()==10);
    REQUIRE(player->equipation().attacks_size() == 1);
    REQUIRE(player->equipation().attacks(0) == PBItemEnum::KICK);
    REQUIRE(player->version() == "0.0.1");
};


TEST_CASE("Inserting location","[db_handler][db_handler_test_2]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"game_state", make_document()};
    del_op_pla.ExecuteOperation();
    PBUser user;
    user.set_discord_id(0);
    user.set_current_player_id(1);
    PBLocation loc;
    auto interaction_1 = loc.add_interactions();
    interaction_1->add_types(PBInteractionType::POST);
    interaction_1->mutable_post_info()->set_capacity_upgrade(10);
    auto interaction_2 = loc.add_interactions();
    interaction_2->add_types(PBInteractionType::ZONE_ACCESS);
    interaction_2->add_types(PBInteractionType::DIALOG);
    db_handler::DBLocationHandler location_handler{&loc};
    REQUIRE(location_handler.InsertNewLocation(user));
    interaction_2->mutable_zone_access_info()->set_unlock_level(200);
    REQUIRE(location_handler.FindPlayerCurrentLocation(user));
    auto post_int = loc.interactions(0);
    REQUIRE(post_int.types_size() == 1);
    REQUIRE(post_int.types(0) == PBInteractionType::POST);
    REQUIRE(post_int.post_info().capacity_upgrade() == 10);
    std::cout << loc.DebugString() << std::endl;

}

TEST_CASE("ModifyItemsQuantity","[db_handler][db_handler_test_3]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"inventory", make_document()};
    del_op_pla.ExecuteOperation();

    std::vector<PBItemData> data;
    db_handler::DBInventoryHandler inventory{&data};
    PBUser user;
    user.set_discord_id(2);
    user.set_current_player_id(1);
    inventory.InitializeInventory(user);
    PBItemData item1;
    item1.set_item_id(PBItemEnum::STICK);
    item1.set_quantity(20);
    item1.set_subtract(false);
    data.push_back(item1);

    PBItemData item2;
    item2.set_item_id(PBItemEnum::PEBBLE);
    item2.set_quantity(12);
    item2.set_subtract(false);
    data.push_back(item2);

    PBItemData item3;
    item3.set_item_id(PBItemEnum::DIRT);
    item3.set_quantity(12);
    item3.set_subtract(false);
    data.push_back(item3);

    REQUIRE(inventory.UpdateItems(user.discord_id(),user.current_player_id()));
    data.at(0).set_quantity(32);
    data.erase(data.begin() + 2);
    REQUIRE(inventory.GetItems(user.discord_id(),user.current_player_id(),PBItemType_Name(PBItemType::RESOURCES)));
    REQUIRE(data.size() == 2);
    REQUIRE(data.at(0).quantity() == 20);

    REQUIRE(inventory.GetInventory(user.discord_id(),user.current_player_id(),PBItemType_Name(PBItemType::RESOURCES)));
    REQUIRE(data.size() == 3);
    data.at(0).set_quantity(200);
    REQUIRE(inventory.UpdateItems(user.discord_id(),user.current_player_id()));
}

TEST_CASE("Update post","[db_handler][db_handler_test_4]")
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"game_state", make_document()};
    del_op_pla.ExecuteOperation();

    auto const *location_data = DCLData::DCLMap::getInstance().GetLocation(PBLocationID::MAIN_BASE);
    auto const *interaction_data = location_data->GetInteraction(0);
    auto const *post_interaction = interaction_data->TryGetPost();
    PBUser user_db;
    user_db.add_players();
    PBPlayer *player = user_db.mutable_players(0);

    PBLocation loc_db;
    db_handler::DBLocationHandler location_handler{&loc_db};
    auto *interaction_db = loc_db.add_interactions();
    interaction_db->add_types(PBInteractionType::POST);
    auto *post_db = interaction_db->mutable_post_info();
    location_handler.InsertNewLocation(user_db);
    
    auto last_collected = post_db->mutable_last_collected();
    auto now = std::chrono::system_clock::now();
    now -= std::chrono::hours(10);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    
    last_collected->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));

    std::vector<PBItemData> item_data = post_interaction->CalculatePostRewards(player,post_db);
    for (auto const &item : item_data)
    {
        std::cout << item.quantity() << std::endl;
    }
    
    REQUIRE(location_handler.UpdateInteraction(0,0,user_db,PBInteractionType::POST));
}