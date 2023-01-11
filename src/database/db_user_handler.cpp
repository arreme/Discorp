#include <db_user_handler.hpp>

bool db_user::DeleteUser(uint64_t user_id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    json j;
    j["discord_id"] = user_id;
    DeleteOneOperation op = DeleteOneOperation("farmer",to_string(j));
    DB_ERR err = access.ExecuteOperation(op);
    return err == DB_ERR::SUCCESS;
}

bool db_user::SaveUser(User user) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto db = (*dbClient)["daw"];
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    InsertOneOperation op = InsertOneOperation("farmer",user.ToJson());
    DB_ERR err = access.ExecuteOperation(op);
    return err == DB_ERR::SUCCESS;
}

std::unique_ptr<User> db_user::FindUserById(uint64_t user_id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    mongocxx::client &client = MongoDBAccess
    auto db = client[DATABASE_NAME];
    json j;
    j["discord_id"] = user_id;
    
    FindOneOperation op = FindOneOperation("farmer",to_string(j));
    access.ExecuteOperation(op);
    
    if (op.IsResult()) 
    {
        json o = json::parse(op.GetResult());
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

    //auto result = access.FindMany(to_string(j),"farmer");
    return {};
    //TODO: For each vector create an user
}