#include <game_manager.hpp>
#include <user.hpp>
#include <farm.hpp>
#include <db_access.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>

gm::gm_err_code gm::CreateGame(uint64_t discord_id, std::string user_name) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    User user = User(discord_id,user_name);
    Farm farm = Farm(discord_id);
    Transaction t;
    auto insertUser = InsertOneOperation("farmer",user.ToJson());
    auto insertFarm = InsertOneOperation("farm",farm.ToJson());
    t.AddOperation(&insertUser);
    t.AddOperation(&insertFarm);
    DB_ERR err_code = access.ExecuteTransaction(t);
    if (err_code == DB_ERR::SUCCESS) 
    {
        return gm_err_code::SUCCESS;
    }
    return gm_err_code::FORMAT_ERROR;
}

gm::gm_err_code gm::DeleteGame(uint64_t discord_id) 
{
    return gm_err_code::SUCCESS;
}

