#include <catch2/catch_test_macros.hpp>
#include <core/pb/player.pb.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>

TEST_CASE("User Database Handler Testing","[db_handler][db_handler_test_1]") 
{   
    PBUser user;
    user.set_discord_id(0);
    user.set_user_name("Arreme");
    user.set_current_player_id(0);
    auto last_online = user.mutable_last_online();
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    last_online->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
    std::cout << google::protobuf::util::TimeUtil::ToString(user.last_online()) << std::endl;
    auto player = user.add_players();
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
    auto interaction_2 = loc.add_interactions();
    interaction_2->add_types(PBInteractionType::ZONE_ACCESS);
    interaction_2->add_types(PBInteractionType::DIALOG);
    db_handler::DBLocationHandler location_handler{&loc};
    REQUIRE(location_handler.InsertNewLocation(user));
}