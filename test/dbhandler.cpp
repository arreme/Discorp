#include <catch2/catch_test_macros.hpp>
#include <db_access.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <user.hpp>
#include <db_query.hpp>
#include <player.hpp>
#include <location.hpp>

using bsoncxx::builder::basic::kvp;

SCENARIO("Database handlers and queries, [db]") 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    GIVEN("1.- A database with a Player") 
    {
        Player pl_1(1,0);
        InsertOneOperation ins_op_1 = InsertOneOperation("players",pl_1.ToJson());
        access.ExecuteOperation(ins_op_1);
        WHEN("1.1.- Pushing a new location") 
        {
            auto loc_doc = bsoncxx::builder::basic::make_document(kvp("locations",Location::LocationBuilder(g_enums::GameLocations::FOREST).ToJson()));
            auto update_doc = bsoncxx::builder::basic::document{};
            update_doc.append(kvp("$push",loc_doc));
            auto query_doc = bsoncxx::builder::basic::document{};
            query_doc.append(kvp("discord_id",1));
            query_doc.append(kvp("player_id",0));
            UpdateOneOperation op = UpdateOneOperation("players",query_doc.extract(),update_doc.extract());
            access.ExecuteOperation(op);
        }
        WHEN("4.2.- Returning player with current location") 
        {

        }
    }
}