#include <catch2/catch_test_macros.hpp>
#include <game_manager.hpp>
#include <game_action.hpp>

SCENARIO("/start", "[command]") 
{
    GIVEN("A user not registered") 
    {
        WHEN("Issuing the command") 
        {
            auto result = db::CreateGameTransaction(User(2,"user_test"),Player(2,0));
            THEN("Initial state should be loaded to database") 
            {

            }
        }
    }
}

SCENARIO("/goto <zone_name>", "[command]") 
{

}

SCENARIO("/unlock <zone_name>", "[command]") 
{

}

SCENARIO("/unlock check <zone_name>", "[command]") 
{

}

SCENARIO("/photo", "[command]") 
{

}

SCENARIO("/mine <post_name>", "[command]") 
{

}

SCENARIO("/upgrade <post_name> <upgrade>", "[command]") 
{

}

SCENARIO("/upgrade show <post_name>", "[command]") 
{

}

SCENARIO("/inventory show", "[command]") 
{

}