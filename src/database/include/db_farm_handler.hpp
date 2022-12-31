#pragma once
#include <db_access.hpp>
#include <db_instance.hpp>
#include <json.hpp>
#include <farm.hpp>
#include <discorp_config.hpp>

using json = nlohmann::json;

namespace db_farm 
{
    bool DeleteFarm(uint64_t discord_id);

    bool SaveFarm(Farm farm);

    std::unique_ptr<Farm> FindFarmByUserId(uint64_t discord_id);
}