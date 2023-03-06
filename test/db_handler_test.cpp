#include <catch2/catch_test_macros.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/player.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>

using namespace bsoncxx::types;
using bsoncxx::builder::basic::kvp;

SCENARIO("User Testing","[db][user]") 
{
    db::DeleteManyOperation del_op = db::DeleteManyOperation("users", bsoncxx::builder::basic::make_document());
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
            auto doc = bsoncxx::builder::basic::make_document(
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
    db::DeleteManyOperation del_op = db::DeleteManyOperation("players", bsoncxx::builder::basic::make_document());
    GIVEN("1.- An empty player database") 
    {
        del_op.ExecuteOperation();
        WHEN("1.1.- Adding a new player") 
        {
            uint64_t discord_id = 2;
            int32_t player_id = 0;
            Player player{2,0};
            
        }
    }
}