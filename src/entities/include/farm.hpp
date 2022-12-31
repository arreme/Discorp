#pragma once
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

class Farm 
{
private:
    uint64_t discord_id;
    //Defines how big and how many plots you have in your farm
    uint8_t field_level;
    //Town mechanics unlocked?
    bool is_road_built;
    //Time reduction for level
    uint8_t soil_level;
    //How many crops can you water
    uint8_t water_tank_level;
public:

    Farm(uint64_t discord_id);

    Farm(uint64_t discord_id, uint8_t fieldlvl, bool road, uint8_t soil_level, uint8_t water_tank);

    uint8_t GetMaxField();

    uint8_t GetFieldNumberSize(uint8_t nPlot);

    uint64_t GetId();

    uint8_t GetFieldLevel();

    bool GetRoadBuilt();

    uint8_t GetSoilLevel();

    uint8_t GetWaterTankLevel();

    std::string ToJson();

};

