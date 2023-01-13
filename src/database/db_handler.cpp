#include <db_handler.hpp>

using namespace db;

bool CreateGameTransaction(User user, Player player) 
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

std::unique_ptr<User> FindUser(uint64_t id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);

    json j;
    j["discord_id"] = id;
    std::string user_str = access.FindOne(to_string(j),"users");
    if (!user_str.empty()) 
    {
        return std::make_unique<User>(User(user_str));
    }
    return nullptr;
}