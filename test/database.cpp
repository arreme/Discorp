#include <catch2/catch_test_macros.hpp>
#include <db_access.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <user.hpp>
#include <db_query.hpp>
#include <player.hpp>
#include <location.hpp>

SCENARIO("Testing write to database","[db]") 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    GIVEN("1.- A new empty user collection database") 
    {
        auto doc_1 = bsoncxx::builder::basic::make_document();
        DeleteManyOperation del_op_1 = DeleteManyOperation("users", doc_1);
        access.ExecuteOperation(del_op_1);
        
        WHEN("1.1.- FindingUser with an empty database") 
        {
            auto doc_1_1 = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));
            FindOneOperation find_op_1_1 = FindOneOperation("users",doc_1_1);
            access.ExecuteOperation(find_op_1_1);
            THEN("Result should be null") 
            {
                REQUIRE_FALSE(find_op_1_1.result);
            }
        }
        WHEN("1.2.- Adding a new user to the database") 
        {
            User user_1_2(0, "test_user");
            InsertOneOperation ins_op_1_2 = InsertOneOperation("users", user_1_2.ToJson());
            THEN("Result should be true") 
            {
                REQUIRE(access.ExecuteOperation(ins_op_1_2));
            }
        }
        WHEN("1.3.- Removing a non existing user from the database") 
        {
            auto doc_1_3 = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));

            DeleteOneOperation del_op_1_3 = DeleteOneOperation("users",doc_1_3);
            THEN("The result should give true") 
            {
                REQUIRE(access.ExecuteOperation(del_op_1_3));
            }
        }
    }

    GIVEN("2.- A database with an user") 
    {
        User user_2 = User(1,"test_user2");

        InsertOneOperation ins_op_2 = InsertOneOperation("users",user_2.ToJson());
        access.ExecuteOperation(ins_op_2);

        WHEN("2.1- Adding the same user id to the database")
        {
            InsertOneOperation ins_op_2_1 = InsertOneOperation("users",user_2.ToJson());
            THEN("Result should be false") 
            {
                REQUIRE_FALSE(access.ExecuteOperation(ins_op_2_1));
            }
        }
        WHEN("2.2.- Finding an existing user") 
        {
            auto doc_2_2 = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",1));
            FindOneOperation find_op_2_2 = FindOneOperation("users", doc_2_2);
            access.ExecuteOperation(find_op_2_2);
            THEN("Result should not be null") 
            {
                REQUIRE(find_op_2_2.result);
                User found_user_2_2 = User(find_op_2_2.result.value().view());
                REQUIRE(found_user_2_2.GetId() == 1);
            }
        }
        WHEN("2.3.- Removing a user from the database")
        {
            auto doc_2_3 = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",1));
            DeleteOneOperation del_op_2_3 = DeleteOneOperation("users",doc_2_3);
            THEN("The user should have been deleted") 
            {
                REQUIRE(access.ExecuteOperation(del_op_2_3));
                auto doc_2_3 = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",1));
                FindOneOperation find_op_2_3 = FindOneOperation("users", doc_2_3);
                access.ExecuteOperation(find_op_2_3);
                REQUIRE_FALSE(find_op_2_3.result);
            }
        }
    }

    GIVEN("3.- A new empty player collection database") 
    {
        auto doc_3 = bsoncxx::builder::basic::make_document();
        DeleteManyOperation del_op_3 = DeleteManyOperation("players", doc_3);
        access.ExecuteOperation(del_op_3);
        WHEN("3.1.- Adding a new player to the database") 
        {
            Player pl_3_1(0,0);
            InsertOneOperation ins_op_3_1 = InsertOneOperation("players",pl_3_1.ToJson());
            THEN("Result should be true") 
            {
                access.ExecuteOperation(ins_op_3_1);
            }
        }
    }
}