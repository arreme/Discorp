#include <catch2/catch_test_macros.hpp>
#include <db_instance.hpp>
#include <db_user_handler.hpp>

SCENARIO("Database basic testing for user","[db]") 
{
    GIVEN("A new empty user collection database") 
    {
        WHEN("FindingUserById with an empty database") 
        {
            uint64_t id = 0;
            auto result = db_user::FindUserById(id);
            THEN("Result should be null") 
            {
                REQUIRE_FALSE(result);
            }
        }
        WHEN("Adding a new user to the database") 
        {
            uint64_t id = 0;
            std::string user_name = "test_user";
            User user(id, user_name);
            THEN("Result should be true") 
            {
                REQUIRE(db_user::SaveUser(user));
            }
        }
        WHEN("Adding the same user id to the database")
        {
            uint64_t id = 0;
            std::string user_name = "test_user";
            User user(id, user_name);
            THEN("Result should be false") 
            {
                REQUIRE_FALSE(db_user::SaveUser(user));
            }
        }
        WHEN("Finding a an existing user") 
        {
            uint64_t id = 0;
            auto result = db_user::FindUserById(id);
            THEN("Result should be null") 
            {
                REQUIRE(result);
                REQUIRE(result->GetUserName() == "test_user");
            }
        }
        WHEN("Removing a user from the database")
        {
            uint64_t id = 0;
            THEN("The user should have been deleted") 
            {
                REQUIRE(db_user::DeleteUser(id));
                auto result = db_user::FindUserById(id);
                REQUIRE_FALSE(result);
            }
        }
        WHEN("Removing a non existing user from the database") 
        {
            uint64_t id = 0;
            THEN("The result should give false") 
            {
                REQUIRE_FALSE(db_user::DeleteUser(id));
            }
        }
    }
}