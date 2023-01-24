#include <catch2/catch_test_macros.hpp>
#include <db_access.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <user.hpp>
#include <db_query.hpp>
#include <player.hpp>
#include <location.hpp>
#include <db_handler.hpp>

using bsoncxx::builder::basic::kvp;

SCENARIO("Database handlers and transactions, [db]") 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    GIVEN("1.- A collection with a Player") 
    {
        Player pl_1(1,0);
        InsertOneOperation ins_op_1 = InsertOneOperation("players",pl_1.ToJson());
        access.ExecuteOperation(ins_op_1);
        WHEN("1.1.- Pushing a new location") 
        {
            THEN("Result should be true") 
            {
                REQUIRE(db::UnlockNewLocation(1,0,g_enums::GameLocations::FOREST));
            }
        }
        WHEN("1.2.- Returning player with current location") 
        {
            auto result = db::FindPlayerCurrentLocationOnly(1,0);
            THEN("Player should exist with only the current location") 
            {
                REQUIRE(result);
                REQUIRE(result->GetLocations()->size() == 1);
            }
        }
        DeleteManyOperation del_op_1 = DeleteManyOperation("players", bsoncxx::builder::basic::make_document());
        access.ExecuteOperation(del_op_1);
    }

    GIVEN("2.- An empty database") 
    {
        DeleteManyOperation del_op_pl = DeleteManyOperation("players", bsoncxx::builder::basic::make_document());
        access.ExecuteOperation(del_op_pl);
        DeleteManyOperation del_op_usr = DeleteManyOperation("users", bsoncxx::builder::basic::make_document());
        access.ExecuteOperation(del_op_usr);
        WHEN("2.1.- Executing a transaction") 
        {
            auto result = db::CreateGameTransaction(User(2,"user_test"),Player(2,0));
            THEN("We should have both a user and a player in the database") 
            {
                REQUIRE(result);
                auto user_res = db::FindUserById(2);
                REQUIRE(user_res);
                auto player_res = db::FindPlayerCurrentLocationOnly(2,0);
                REQUIRE(player_res);
            }
        }
    }
}