#include <catch2/catch_test_macros.hpp>
#include <db_user_handler.hpp>
#include <db_farm_handler.hpp>
#include <game_manager.hpp>

SCENARIO("Game state tests and setup, [db] [user]") 
{
    GIVEN("A new user not registered to the game") 
    {
        uint64_t id = 1;
        std::string name= "game_user";
        auto result = db_user::FindUserById(id);
        if (result) 
        {
            db_user::DeleteUser(id);
        }        

        WHEN("/dc plant carrotos 1") 
        {
            THEN("Error: User has not registered to the game") 
            {

            }
        }
        
        WHEN("/dc start") 
        {
            gm::gm_err_code code = gm::CreateGame(id, name);
            THEN("Initial user should be created in database") 
            {
                auto result = db_user::FindUserById(id);
                REQUIRE(result);
            }
            THEN("Initial farm should be created in database") 
            {
                auto result = db_farm::FindFarmByUserId(id);
                REQUIRE(result);
            }
        }
        // WHEN("/dc start") 
        // {
        //     gm::gm_err_code code = gm::CreateGame(id, name);
        //     THEN("Error: user already registered") 
        //     {
        //         REQUIRE(code == gm::gm_err_code::USER_ALREADY_ADDED);
        //     }
        // }


    }

}