#include <catch2/catch_test_macros.hpp>
#include <db_access.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <user.hpp>

SCENARIO("Testing write to database","[db]") 
{
    GIVEN("A new empty user collection database") 
    {
        auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
        auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

        auto doc = bsoncxx::builder::basic::make_document();
        DeleteManyOperation op = DeleteManyOperation("users", doc);
        REQUIRE(access.ExecuteOperation(op));
        
        WHEN("FindingUserById with an empty database") 
        {
            auto doc = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0));
            auto user_str = access.FindOne("users",doc);
            THEN("Result should be null") 
            {
                REQUIRE_FALSE(user_str);
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
        auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
        auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

        User user = User(0,"test_user");

        InsertOneOperation op = InsertOneOperation("users",user.ToJson());
        access.ExecuteOperation(op);

        // AND_WHEN("Adding the same user id to the database")
        // {
        //     uint64_t id = 0;
        //     std::string user_name = "test_user";
        //     User user(id, user_name);
        //     THEN("Result should be false") 
        //     {
        //         REQUIRE_FALSE(db_user::SaveUser(user));
        //     }
        // }
        // WHEN("Finding an existing user") 
        // {
        //     uint64_t id = 0;
        //     auto result = db_user::FindUserById(id);
        //     THEN("Result should be null") 
        //     {
        //         REQUIRE(result);
        //         REQUIRE(result->GetUserName() == "test_user");
        //     }
        // }
        // WHEN("Removing a user from the database")
        // {
        //     uint64_t id = 0;
        //     THEN("The user should have been deleted") 
        //     {
        //         REQUIRE(db_user::DeleteUser(id));
        //         auto result = db_user::FindUserById(id);
        //         REQUIRE_FALSE(result);
        //     }
        // }
    }
}