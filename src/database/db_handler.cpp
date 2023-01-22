#include <db_handler.hpp>

bool db::CreateGameTransaction(User user, Player player) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

    Transaction t;
    InsertOneOperation usr_op = InsertOneOperation("users", user.ToJson());
    //InsertOneOperation plr_op = InsertOneOperation("players", player.ToJson());
    t.AddOperation(usr_op);
    //t.AddOperation(plr_op);
    return access.ExecuteTransaction(t);
}

std::unique_ptr<User> db::FindUserById(uint64_t id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    
    FindOneOperation op = FindOneOperation("users",bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0)));
    access.ExecuteOperation(op);
    if (op.result)
    {
        return std::make_unique<User>(User(op.result->view()));
    }
    return nullptr;
}

std::tuple<User,Player,Location> FindPlayerUserAndLocation(uint64_t discord_id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
}