#include <db_handler.hpp>

using namespace bsoncxx::builder::basic;

bool db::CreateGameTransaction(User user, Player player) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

    Transaction t;
    InsertOneOperation usr_op = InsertOneOperation("users", user.ToJson());
    InsertOneOperation plr_op = InsertOneOperation("players", player.ToJson());
    t.AddOperation(usr_op);
    t.AddOperation(plr_op);
    return access.ExecuteTransaction(t);
}

std::unique_ptr<User> db::FindUserById(uint64_t id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

    FindOneOperation op = FindOneOperation("users",bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",bsoncxx::types::b_int64(id))));
    access.ExecuteOperation(op);
    if (op.result)
    {
        return std::make_unique<User>(User(op.result->view()));
    }
    return nullptr;
}

std::unique_ptr<Player> db::FindPlayerCurrentLocationOnly(uint64_t id, uint8_t player_id)
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

    mongocxx::pipeline p{};
    p.match(make_document(kvp("discord_id",bsoncxx::types::b_int64(id)),kvp("player_id",bsoncxx::types::b_int32(player_id))));
    bsoncxx::builder::basic::array array;
    bsoncxx::builder::basic::array arrayElement;
    arrayElement.append("$locations");
    arrayElement.append(make_document(kvp("$getField",bsoncxx::types::b_utf8("current_loc"))));
    array.append(make_document(kvp("$arrayElemAt",arrayElement)));
    p.add_fields(make_document(kvp("locations_project",bsoncxx::types::b_array(array))));
    p.project(make_document(kvp("locations",0)));

    AggregateOperation op = AggregateOperation("players",std::move(p));
    if(access.ExecuteOperation(op)) 
    {
        if(op.result) 
        {   
            return std::make_unique<Player>(*(op.result.value().begin()));
        }
    }
    
    return nullptr;
}

bool db::UnlockNewLocation(uint64_t id, uint8_t player_id, g_enums::GameLocations new_location) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    auto loc_doc = bsoncxx::builder::basic::make_document(kvp("locations",Location::LocationBuilder(g_enums::GameLocations::FOREST).ToJson()));
    auto update_doc = bsoncxx::builder::basic::document{};
    update_doc.append(kvp("$push",loc_doc));
    auto query_doc = bsoncxx::builder::basic::document{};
    query_doc.append(kvp("discord_id",bsoncxx::types::b_int64(id)));
    query_doc.append(kvp("player_id",bsoncxx::types::b_int32(player_id)));
    UpdateOneOperation op = UpdateOneOperation("players",query_doc.extract(),update_doc.extract());
    return access.ExecuteOperation(op);
}