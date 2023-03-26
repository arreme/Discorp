#include <catch2/catch_test_macros.hpp>
#include <core/game_manager.hpp>

using bsoncxx::builder::basic::make_document;

TEST_CASE("/start", "[core]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();

    REQUIRE(gm::CreateGame(0,"Arreme") == gm::Errors::SUCCESS);

    REQUIRE(gm::CreateGame(0,"Arreme") == gm::Errors::DUPLICATED_PK_ERROR);

    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
}

TEST_CASE("/goto <location>", "[goto]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    db::DeleteManyOperation del_op_inv = db::DeleteManyOperation{"inventory", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
    del_op_inv.ExecuteOperation();

    REQUIRE(gm::GoToZone(0,1) == gm::Errors::USER_NOT_FOUND);
    gm::CreateGame(0,"Arreme");
    REQUIRE(gm::GoToZone(0,10) == gm::Errors::INTERACTION_NOT_FOUND);
    REQUIRE(gm::GoToZone(0,0) == gm::Errors::ILLEGAL_ACTION);
    REQUIRE(gm::GoToZone(0,1) == gm::Errors::LOCATION_LOCKED);
    REQUIRE(gm::UnlockZone(0,1) == gm::Errors::NOT_ENOUGH_RESOURCES);
    db_handler::ModifyItemQuantity(0,1,Item::RESOURCE_TYPE,PBResourceItems::STICK,1);
    REQUIRE(gm::UnlockZone(0,1) == gm::Errors::NOT_ENOUGH_RESOURCES);
    db_handler::ModifyItemQuantity(0,1,Item::RESOURCE_TYPE,PBResourceItems::STICK,10);
    REQUIRE(gm::UnlockZone(0,1) == gm::Errors::SUCCESS);
    REQUIRE(gm::GoToZone(0,1) == gm::Errors::SUCCESS);
    REQUIRE(db_handler::CurrentPlayerLocation(0,1) == PBLocationID::FOREST);
    REQUIRE(gm::UnlockZone(0,2) == gm::Errors::SUCCESS);
    REQUIRE(gm::GoToZone(0,2) == gm::Errors::SUCCESS);
    REQUIRE(db_handler::CurrentPlayerLocation(0,1) == PBLocationID::TOWN);
    REQUIRE(gm::GoToZone(0,0) == gm::Errors::SUCCESS);
    REQUIRE(db_handler::CurrentPlayerLocation(0,1) == PBLocationID::FOREST);
    REQUIRE(gm::GoToZone(0,0) == gm::Errors::ILLEGAL_ACTION);
    REQUIRE(gm::GoToZone(0,1) == gm::Errors::SUCCESS);
    REQUIRE(db_handler::CurrentPlayerLocation(0,1) == PBLocationID::MAIN_BASE);

    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
    del_op_inv.ExecuteOperation();
    
}

TEST_CASE("/collect post","[collect]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    db::DeleteManyOperation del_op_inv = db::DeleteManyOperation{"inventory", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
    del_op_inv.ExecuteOperation();

    std::string output = "REWARDS COLLECTED\n-----------------";
    REQUIRE(gm::CollectPost(0,0,output) == gm::Errors::USER_NOT_FOUND);
    gm::CreateGame(0,"Arreme");
    Player player{0, 1, PBLocationID::MAIN_BASE};
    REQUIRE(db_handler_util::ModifyPostDate(player,0,-20));
    gm::CollectPost(0,0,output);
    std::cout << output << std::endl;
    gm::CollectPost(0,0,output);

}