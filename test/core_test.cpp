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
    db_handler::ModifyItemQuantity(0,1,Item::RESOURCE_TYPE,PBItemEnum::STICK,1);
    REQUIRE(gm::UnlockZone(0,1) == gm::Errors::NOT_ENOUGH_RESOURCES);
    db_handler::ModifyItemQuantity(0,1,Item::RESOURCE_TYPE,PBItemEnum::STICK,10);
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
    auto req1 = db_handler::FindPlayerCurrentInteraction(0,1,0);
    REQUIRE(req1->first.GetSkills()->m_forage_xp == 2);
    auto req1_post = static_cast<PostInfo *>(req1->second.get());
    REQUIRE(req1_post->GetResourceStored() == 18);
    auto req1_items = db_handler::GetItem(0,1,Item::RESOURCE_TYPE,PBItemEnum::STICK);
    REQUIRE(req1_items->GetQuantity() == 2);
    gm::CollectPost(0,0,output);
    auto req2 = db_handler::FindPlayerCurrentInteraction(0,1,0);
    REQUIRE(req2->first.GetSkills()->m_forage_xp == 4);
    auto req2_post = static_cast<PostInfo *>(req2->second.get());
    REQUIRE(req2_post->GetResourceStored() == 16);
    auto req2_items = db_handler::GetItem(0,1,Item::RESOURCE_TYPE,PBItemEnum::STICK);
    REQUIRE(req2_items->GetQuantity() == 4);
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
    del_op_inv.ExecuteOperation();

}


TEST_CASE("/upgrade post","[upgrade_post]") 
{
    db::DeleteManyOperation del_op_pla = db::DeleteManyOperation{"players", make_document()};
    db::DeleteManyOperation del_op_usr = db::DeleteManyOperation{"users", make_document()};
    db::DeleteManyOperation del_op_inv = db::DeleteManyOperation{"inventory", make_document()};
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
    del_op_inv.ExecuteOperation();

    gm::CreateGame(0,"Arreme");
    Player player{0, 1, PBLocationID::MAIN_BASE};
    REQUIRE(gm::ImprovePost(0,0,"CAPACITY") == gm::Errors::NOT_ENOUGH_RESOURCES);
    REQUIRE(db_handler_util::ModifyPostDate(player,0,-20));
    std::string output = "REWARDS COLLECTED\n-----------------";
    REQUIRE(gm::CollectPost(0,0,output) == gm::Errors::SUCCESS);
    REQUIRE(gm::CollectPost(0,0,output) == gm::Errors::SUCCESS);
    REQUIRE(gm::ImprovePost(0,0,"CAPACITY") == gm::Errors::SUCCESS);
    REQUIRE(gm::ImprovePost(0,0,"CAPACITY") == gm::Errors::INTERACTION_ALREADY_UNLOCKED);
    
    del_op_pla.ExecuteOperation();
    del_op_usr.ExecuteOperation();
    del_op_inv.ExecuteOperation();
}


TEST_CASE("/inventory","[inventory_core]") 
{
    int size = -1;
    std::string type = "resources";
    auto res = gm::Inventory(196551239025164289,type,0,&size);
}