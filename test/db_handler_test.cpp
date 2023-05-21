#include <catch2/catch_test_macros.hpp>
#include <core/pb/player.pb.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>
#include <db_handler/db_inventory.hpp>
#include <db_handler/db_combat.hpp>

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
    user.add_players()->set_current_location(PBLocationID::MAIN_BASE);
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
    data.push_back(item1);

    PBItemData item2;
    item2.set_item_id(PBItemEnum::PEBBLE);
    item2.set_quantity(12);
    data.push_back(item2);

    PBItemData item3;
    item3.set_item_id(PBItemEnum::DIRT);
    item3.set_quantity(12);
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
    std::vector<PBItemData> dataset2;
    PBItemData mod_item;
    mod_item.set_item_id(PBItemEnum::ROCK);
    mod_item.set_quantity(20);

    PBItemData mod_item2;
    mod_item2.set_item_id(PBItemEnum::STICK);
    mod_item2.set_quantity(-4);
    dataset2.push_back(mod_item);
    dataset2.push_back(mod_item2);

    //REQUIRE(db_handler::DBInventoryHandler::ModifyItemsQuantity(user.discord_id(),user.current_player_id()));
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
    
    REQUIRE(item_data.size() == 1);
    REQUIRE(item_data.at(0).quantity() == 1);
    
    REQUIRE(location_handler.UpdateInteraction(0,0,user_db,PBInteractionType::POST));
}

TEST_CASE("Combat handler testing","[db_handler][db_handler_test_5]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"combat", make_document()};
    del_op_pla.ExecuteOperation();

    PBCombat combat_db;
    db_handler::DBCombatHandler combat_handler{&combat_db};

    combat_db.set_starter_user_id(0);
    combat_db.set_opponent_user_id(1);
    combat_db.set_turn(0);
    combat_db.set_wager(10);
    PBPlayer *player1 = combat_db.mutable_starter_user_info();
    player1->mutable_stats()->set_max_health(100);
    player1->mutable_stats()->set_current_health(50);
    PBPlayer *player2 = combat_db.mutable_opponent_user_info();
    player2->mutable_stats()->set_max_health(100);
    player2->mutable_stats()->set_current_health(50);
    player2->mutable_skills()->set_foraging_lvl(10);

    REQUIRE_FALSE(combat_handler.FindCurrentCombat());
    REQUIRE(combat_handler.InsertNewCombat());
    combat_db.set_wager(10000);
    REQUIRE(combat_handler.FindCurrentCombat());
    REQUIRE(combat_db.wager() == 10);

    REQUIRE(combat_handler.UpdateCombatForPlayer(true,true));
    REQUIRE(combat_handler.DeleteCombat());
    REQUIRE_FALSE(combat_handler.FindCurrentCombat());

}