#include <db_farm_handler.hpp>

bool db_farm::SaveFarm(Farm farm) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    InsertOneOperation op = InsertOneOperation("farm",farm.ToJson());
    DB_ERR err = access.ExecuteOperation(op);
    return err==DB_ERR::SUCCESS;
}

bool db_farm::DeleteFarm(uint64_t id) 
{
    return false;
}

std::unique_ptr<Farm> db_farm::FindFarmByUserId(uint64_t id) 
{
    auto dbClient = MongoDBInstance::GetInstance()->getClientFromPool();
    auto access = MongoDBAccess(*dbClient,DATABASE_NAME);
    json j;
    j["discord_id"] = id;
    FindOneOperation op = FindOneOperation("farm",to_string(j));
    access.ExecuteOperation(op);
    
    if (op.IsResult()) 
    {
        json o = json::parse(op.GetResult());
        uint8_t field_level = o["field_level"];
        bool road_built = o["road_built"];
        uint8_t soil_level = o["soil_level"];
        uint8_t water_level = o["water_level"];
        return std::make_unique<Farm>(Farm(id,field_level,road_built,soil_level,water_level));
    }

    return nullptr;
}  