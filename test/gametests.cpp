#include <catch2/catch_test_macros.hpp>
#include <game_manager.hpp>
#include <game_action.hpp>

SCENARIO("/start", "[command]") 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    GIVEN("A user not registered") 
    {
        DeleteManyOperation del_op_pl = DeleteManyOperation("players", bsoncxx::builder::basic::make_document());
        access.ExecuteOperation(del_op_pl);
        DeleteManyOperation del_op_usr = DeleteManyOperation("users", bsoncxx::builder::basic::make_document());
        access.ExecuteOperation(del_op_usr);
        WHEN("Issuing the command") 
        {
            auto result = gm::CreateGame(3,"game_player");
            THEN("The result should be true and initial state should be loaded") 
            {
                REQUIRE(result == Errors::SUCCESS);

                auto usr_res = db::FindUserById(3);
                REQUIRE(usr_res);
                REQUIRE(usr_res->GetId() == 3);
                REQUIRE(usr_res->GetUserName() == "game_player");
                REQUIRE(usr_res->GetCurrentPlayer() == 1);
                REQUIRE_FALSE(usr_res->HasDLC());

                auto ply_res = db::FindPlayerByIdAndCurrent(3,1);
                REQUIRE(ply_res);
                REQUIRE(ply_res->GetSkills()->m_combat_lvl == 1);
                auto locs = *ply_res->GetLocations();
                REQUIRE(locs.size() == 1);
                REQUIRE(locs[0].GetLocId() == g_enums::GameLocations::MAIN_BASE);
            }
        }
    }
    GIVEN("A user already registered") 
    {
        WHEN("Issuing the command") 
        {
            auto result = gm::CreateGame(3,"game_player");
            THEN("TEMPORAL: The result should be false") 
            {
                REQUIRE(result == Errors::DUPLICATED_PK_ERROR);
            }
        }
    }
}

SCENARIO("/photo", "[command]") 
{
    WHEN("Player is in Main Zone") 
    {
        gm::PhotoCurrentLocation(3);
        THEN("A photo of the main zone should be generated") 
        {

        }
    }
}

SCENARIO("/mine <post_name>", "[command]") 
{

}

SCENARIO("/unlock <zone_name>", "[command]") 
{

}

SCENARIO("/goto <zone_name>", "[command]") 
{
    GIVEN("The Zone is unlocked")
    {
        WHEN("/goto Forest")
        {
            
        }
    }
}

SCENARIO("/unlock check <zone_name>", "[command]") 
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