#include <catch2/catch_test_macros.hpp>
#include <db_access.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <user.hpp>
#include <db_query.hpp>
#include <player.hpp>

SCENARIO("Testing write to database","[db]") 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    GIVEN("A new empty user collection database") 
    {
        auto doc = bsoncxx::builder::basic::make_document();
        DeleteManyOperation op = DeleteManyOperation("users", doc);
        REQUIRE(access.ExecuteOperation(op));
        
        WHEN("FindingUser with an empty database") 
        {
            auto doc = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));
            FindOneOperation op = FindOneOperation("users",doc);
            access.ExecuteOperation(op);
            THEN("Result should be null") 
            {
                REQUIRE_FALSE(op.result);
            }
        }
        WHEN("Adding a new user to the database") 
        {
            uint64_t id = 0;
            std::string user_name = "test_user";
            User user(id, user_name);
            
            InsertOneOperation usr_op = InsertOneOperation("users", user.ToJson());
            THEN("Result should be true") 
            {
                REQUIRE(access.ExecuteOperation(usr_op));
            }
        }
        WHEN("Removing a non existing user from the database") 
        {
            auto doc = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));

            DeleteOneOperation del_user = DeleteOneOperation("users",doc);
            THEN("The result should give true") 
            {
                REQUIRE(access.ExecuteOperation(del_user));
            }
        }
    }

    GIVEN("A database with an user") 
    {
        User user = User(0,"test_user");

        InsertOneOperation op = InsertOneOperation("users",user.ToJson());
        access.ExecuteOperation(op);

        WHEN("Adding the same user id to the database")
        {
            THEN("Result should be false") 
            {
                REQUIRE_FALSE(access.ExecuteOperation(op));
            }
        }
        WHEN("Finding an existing user") 
        {
            auto doc = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));
            FindOneOperation usr_op = FindOneOperation("users", doc);
            access.ExecuteOperation(usr_op);
            THEN("Result should not be null") 
            {
                REQUIRE(usr_op.result);
                User found_user = User(usr_op.result.value().view());
                REQUIRE(found_user.GetId() == 0);
            }
        }
        WHEN("Removing a user from the database")
        {
            auto doc = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));
            DeleteOneOperation usr_del_op = DeleteOneOperation("users",doc);
            THEN("The user should have been deleted") 
            {
                REQUIRE(access.ExecuteOperation(usr_del_op));
            }
        }
    }

    GIVEN("A new empty player collection database") 
    {
        auto doc = bsoncxx::builder::basic::make_document();
        DeleteManyOperation op = DeleteManyOperation("players", doc);
        //REQUIRE(access.ExecuteOperation(op));
        WHEN("Adding a new player to the database") 
        {
            Player pl = Player(0,"my_player",0);
            InsertOneOperation pl_op = InsertOneOperation("players",pl.ToJson());
            THEN("Result should be true") 
            {
                access.ExecuteOperation(pl_op);
            }
        }
    }
}