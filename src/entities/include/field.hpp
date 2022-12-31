#pragma once
#include <iostream>
#include <vector>
#include <plant.hpp>

class Field 
{
private:
    uint64_t discord_id;
    uint8_t field_number;
    uint8_t field_size;
    std::vector<Plant> field;
public:
    void PlantCrop(uint8_t plot) 
    {

    }

    void HarvestCrop(uint8_t plot) 
    {

    }

    void TriggerEffect(uint8_t plot) 
    {

    }
};

