#include <catch2/catch_test_macros.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/player.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <db_handler/entities/interaction.hpp>
#include <db_handler/db_handler.hpp>


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
            Player player{1,0};

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
        Player player{2,0};

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
                kvp("discord_id", 2),
                kvp("player_id",  0)
            );

            db::FindOneOperation find_op_2_2{"players",std::move(doc)};
            THEN("Result should give a player obj") 
            {
                find_op_2_2.ExecuteOperation();
                REQUIRE(find_op_2_2.m_result);
                Player player_2{find_op_2_2.m_result->view()};
                REQUIRE(player_2.GetLocation() == g_enums::GameLocations::MAIN_BASE);
                REQUIRE(player_2.GetSkills()->m_forage_lvl == 1);
                REQUIRE(player_2.GetStats()->m_luck == 0);
            }
        }

        del_op.ExecuteOperation();
    }
}

SCENARIO("Database handler","[db][handler]") 
{
    db::DeleteManyOperation del_op = db::DeleteManyOperation{"players", make_document()};
    GIVEN("1.- A user with no locations (fresh start)") 
    {
        uint64_t discord_id = 2;
        int32_t player_id = 0;
        Player player{2,0};

        std::vector<InteractionInfo *> interactionInfo;
        PostInfo post{0};
        ZoneAccessInfo zoneAccessInfo{1};
        interactionInfo.push_back(&post);
        interactionInfo.push_back(&zoneAccessInfo);
        interactionInfo.push_back(&post);
        WHEN("1.1.- Inserting new user to database") 
        {
            db_handler::RegisterPlayerToDatabase(player,interactionInfo);
        }


    }

    GIVEN("2.- A user with MAIN BASE location and One interaction") 
    {

    }
}