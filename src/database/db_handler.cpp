#include <db_handler.hpp>

using namespace db;

void CreateGameTransaction(User user, Player player) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

    Transaction t;
    InsertOneOperation usr_op = InsertOneOperation("users",user.ToJson());
    InsertOneOperation plr_op = InsertOneOperation("players",player.ToJson());
    t.AddOperation(usr_op);
    t.AddOperation(plr_op);
    access.ExecuteTransaction(t);
}