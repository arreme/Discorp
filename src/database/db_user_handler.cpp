#include <db_user_handler.hpp>

bool db_user::DeleteUser(uint64_t user_id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    json j;
    j["discord_id"] = user_id;
    int result = access.DeleteOne(to_string(j),"farmer");
    return result > 0;
}

bool db_user::SaveUser(User user) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    int result = access.InsertOne(user.ToJson(), "farmer");
    return result==0;
}

std::unique_ptr<User> db_user::FindUserById(uint64_t user_id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    json j;
    j["discord_id"] = user_id;
    std::string result = access.FindOne(to_string(j),"farmer");
    
    if (!result.empty()) 
    {
        json o = json::parse(result);
        std::string user_name = o["user_name"];
        return std::make_unique<User>(User(user_id,user_name));
    }

    return nullptr;
}

std::unique_ptr<User[]> db_user::FindUsersById(uint64_t user_id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    json j;
    j["discord_id"] = user_id;
    auto result = access.FindMany(to_string(j),"farmer");
    return {};
    //TODO: For each vector create an user
}