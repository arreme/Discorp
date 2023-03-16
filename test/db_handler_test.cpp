#include <catch2/catch_test_macros.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/player.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <db_handler/entities/interaction.hpp>
#include <db_handler/db_handler.hpp>
#include <core/pb/location.pb.h>


using namespace bsoncxx::types;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

SCENARIO("User Testing","[db][user]") 
{
    db::DeleteManyOperation del_op = db::DeleteManyOperation{"users", make_document()};
    GIVEN("1.- An empty user database") 
    {
        del_op.ExecuteOperation();

        WHEN("1.1.- Adding a new user") 
        {
            User user{1,"test"};
            db::InsertOneOperation ins_op_1{"users",user.ToJson()};

            THEN("Result should be true") 
            {
                ins_op_1.ExecuteOperation();
                REQUIRE(ins_op_1.GetState() == db::OperationState::SUCCESS);
            }
        }
        WHEN("1.2.- Finding a user") 
        {
            uint64_t id = 0;
            auto doc = make_document(
                kvp("id",b_int64{static_cast<int64_t>(id)})
            );
            db::FindOneOperation find_op_1{"users",std::move(doc)};
            THEN("Result should be null") 
            {
                find_op_1.ExecuteOperation();
                REQUIRE_FALSE(find_op_1.m_result);
            }
        }
    }
    GIVEN("2.- A database with an user") 
    {
        User user{1,"test"};
        db::InsertOneOperation ins_op_2{"users",user.ToJson()};
        ins_op_2.ExecuteOperation();
        WHEN("2.1.- Adding the same user new user") 
        {
            THEN("Result should be true") 
            {
                ins_op_2.ExecuteOperation();
                REQUIRE(ins_op_2.GetState() == db::OperationState::DUPLICATED_ID);
            }
        }
        WHEN("2.2.- Retrieving a user from a database") 
        {
            uint64_t id = 1;
            auto doc = bsoncxx::builder::basic::make_document(
                kvp("discord_id",b_int64{static_cast<int64_t>(id)})
            );
            db::FindOneOperation find_op_2{"users",std::move(doc)};
            THEN("Result should contain a user") 
            {
                find_op_2.ExecuteOperation();
                REQUIRE(find_op_2.m_result);
                User user {find_op_2.m_result->view()};
                REQUIRE(user.GetUserName() == "test");
                REQUIRE(user.GetId() == 1);
                REQUIRE(user.GetCurrentPlayer() == 1);
                REQUIRE_FALSE(user.HasDLC());
            }
        }

        del_op.ExecuteOperation();
    }
}

SCENARIO("Player testing","[db][player]") 
{
    db::DeleteManyOperation del_op = db::DeleteManyOperation{"players", make_document()};
    GIVEN("1.- An empty player database") 
    {
        del_op.ExecuteOperation();
        WHEN("1.1.- Adding a new player") 
        {
            uint64_t discord_id = 1;
            int32_t player_id = 0;
            Player player{1,0,PBLocationID::MAIN_BASE};

            db::InsertOneOperation ins_op_1{"players",player.ToJson()};
            THEN("Result should be true") 
            {
                ins_op_1.ExecuteOperation();
                REQUIRE(ins_op_1.GetState() == db::OperationState::SUCCESS);
            }
        }
        del_op.ExecuteOperation();
    }
    
    GIVEN("2.- A database with a player") 
    {
        uint64_t discord_id = 2;
        int32_t player_id = 0;
        Player player{2,0,PBLocationID::MAIN_BASE};

        db::InsertOneOperation ins_op_2{"players",player.ToJson()};
        ins_op_2.ExecuteOperation();
        WHEN("2.1.- Adding a the same player") 
        {
            ins_op_2.ExecuteOperation();
            THEN("Result should be repeated ID") 
            {
                REQUIRE(ins_op_2.GetState() == db::OperationState::DUPLICATED_ID);
            }
        }
        WHEN("2.2.- Finding a player") 
        {
            auto doc = make_document(
                kvp("discord_id", b_int64{static_cast<int64_t>(2)}),
                kvp("player_id",  b_int32{0})
            );

            db::FindOneOperation find_op_2_2{"players",std::move(doc)};
            THEN("Result should give a player obj") 
            {
                find_op_2_2.ExecuteOperation();
                REQUIRE(find_op_2_2.m_result);
                Player player_2{find_op_2_2.m_result->view()};
                REQUIRE(player_2.GetLocation() == PBLocationID::MAIN_BASE);
                REQUIRE(player_2.GetSkills()->m_forage_lvl == 1);
                REQUIRE(player_2.GetStats()->m_luck == 0);
            }
        }

        del_op.ExecuteOperation();
    }
}

TEST_CASE("Database Handler: Change Active Player and Find User","[handler][change_player]") 
{
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    del_op_usr.ExecuteOperation();
    uint64_t discord_id = 1;
    int32_t newPlayer = 10;
    SECTION("No user registered") 
    {
        REQUIRE_FALSE(db_handler::ChangeActivePlayer(discord_id,newPlayer));
        REQUIRE_FALSE(db_handler::FindUserById(discord_id));
    }
    SECTION("User registered") 
    {
        User user{discord_id,"Arreme"};
        db::InsertOneOperation ins_op{"users",user.ToJson()};
        ins_op.ExecuteOperation();
        REQUIRE(ins_op.GetState() == db::OperationState::SUCCESS);
        REQUIRE(db_handler::ChangeActivePlayer(discord_id,newPlayer));
        auto result = db_handler::FindUserById(discord_id);
        REQUIRE(result);
        REQUIRE(result->GetUserName() == "Arreme");
        REQUIRE(result->GetCurrentPlayer() == newPlayer);
    }

    del_op_usr.ExecuteOperation();
}

TEST_CASE("Database Handler: Register Player To Database and Find interactions", "[handler][player_register]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();

    uint64_t discord_id = 2;
    User user{discord_id,"Arreme"};
    Player player{discord_id,user.GetCurrentPlayer(),PBLocationID::MAIN_BASE};

    std::vector<std::reference_wrapper<InteractionInfo>> interactionInfo;
    PostInfo post{};
    ZoneAccessInfo zoneAccessInfo{};
    interactionInfo.push_back(post);
    interactionInfo.push_back(zoneAccessInfo);
    interactionInfo.push_back(post);

    auto result = db_handler::RegisterPlayerToDatabase(user, player,interactionInfo);
    REQUIRE(result);
    SECTION("FindPlayerInteractions") 
    {
        auto res = db_handler::FindPlayerCurrentLocationInteractions(discord_id,user.GetCurrentPlayer());
        auto usr = db_handler::FindUserById(discord_id);
        REQUIRE(usr);
        REQUIRE(usr.value().GetUserName() == "Arreme");
        REQUIRE(res);
        REQUIRE(static_cast<PBLocationID>(res.value().first.GetLocation()) == PBLocationID::MAIN_BASE);
        REQUIRE(res.value().second.size() == 3);
        REQUIRE(res.value().second[0].get()->GetType() == InteractionType::POST);
    }
    SECTION("FindPlayerCurrentInteraction") 
    {
        REQUIRE(result);
        auto res = db_handler::FindPlayerCurrentInteraction(discord_id,user.GetCurrentPlayer(),1);
        REQUIRE(res);
        REQUIRE(res.value().first.GetLocation() == PBLocationID::MAIN_BASE);
        REQUIRE(res.value().second.get()->GetType() == InteractionType::ZONE_ACCESS);
    }

    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
}

TEST_CASE("Database Handler: Go to location and unlock location","[handler][unlock_location]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();

    uint64_t discord_id = 2;
    User user{discord_id,"Arreme"};
    Player player{discord_id,user.GetCurrentPlayer(),PBLocationID::MAIN_BASE};

    std::vector<std::reference_wrapper<InteractionInfo>> interactionInfo;
    PostInfo post{};
    ZoneAccessInfo zoneAccessInfo{};
    interactionInfo.push_back(post);
    interactionInfo.push_back(zoneAccessInfo);
    interactionInfo.push_back(post);

    auto result = db_handler::RegisterPlayerToDatabase(user, player,interactionInfo);
    REQUIRE(result);

    std::vector<std::reference_wrapper<InteractionInfo>> newInteractionInfo;
    
    newInteractionInfo.push_back(zoneAccessInfo);
    newInteractionInfo.push_back(post);
    newInteractionInfo.push_back(zoneAccessInfo);
    newInteractionInfo.push_back(post);
    REQUIRE(db_handler::UnlockLocation(player,1,PBLocationID::FOREST,newInteractionInfo));
    REQUIRE(db_handler::GoToLocation(player,PBLocationID::FOREST));

    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
}

TEST_CASE("Database Handler: Collect a post and upgrade a post","[handler][update_post]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();

    uint64_t discord_id = 2;
    User user{discord_id,"Arreme"};
    Player player{discord_id,user.GetCurrentPlayer(),PBLocationID::MAIN_BASE};

    std::vector<std::reference_wrapper<InteractionInfo>> interactionInfo;
    PostInfo post{};
    ZoneAccessInfo zoneAccessInfo{};
    interactionInfo.push_back(post);
    interactionInfo.push_back(zoneAccessInfo);
    interactionInfo.push_back(post);
    auto result = db_handler::RegisterPlayerToDatabase(user, player, interactionInfo);
    REQUIRE(result);
    REQUIRE(db_handler::CollectPost(player,0));
    REQUIRE(db_handler::ImprovePost(player,0,"capacity_lvl"));
    auto interaction_res = db_handler::FindPlayerCurrentInteraction(discord_id,user.GetCurrentPlayer(),0);
    auto post_result = static_cast<PostInfo *>(interaction_res->second.get());
    REQUIRE(post_result->GetCapacityLvl() == 1);

    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
}