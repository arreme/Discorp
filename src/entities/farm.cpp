#include <farm.hpp>

Farm::Farm(uint64_t discord_id) 
: Farm(discord_id,0,false,0,0)
{
    
}

Farm::Farm(uint64_t discord_id, uint8_t fieldlvl, bool road, uint8_t soil_level, uint8_t water_tank)
: discord_id(discord_id), field_level(fieldlvl), is_road_built(road), soil_level(soil_level), water_tank_level(water_tank)
{

}

uint8_t Farm::GetMaxField() 
{
    return (field_level / 5) + 1;
}

uint8_t Farm::GetFieldNumberSize(uint8_t nPlot) 
{
    if ((field_level / 5) + 1 > nPlot) return 4;
    else return field_level%5;
}

uint64_t Farm::GetId()
{
    return discord_id;
}

uint8_t Farm::GetFieldLevel()  
{
    return field_level;
}

bool Farm::GetRoadBuilt() 
{
    return is_road_built;
}

uint8_t Farm::GetSoilLevel() 
{
    return soil_level;
}

uint8_t Farm::GetWaterTankLevel() 
{
    return water_tank_level;
}

std::string Farm::ToJson() 
{
    json j;
    j["discord_id"] = discord_id;
    j["soil_level"] = soil_level;
    j["water_level"] = water_tank_level;
    j["field_level"] = field_level;
    j["road_built"] = is_road_built;
    return to_string(j);
}

