#pragma once
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;


struct Stats
{
    uint8_t farming;
    uint8_t fishing;
    uint8_t mining;
    uint8_t foraging;   
};

class User 
{
private:
    uint64_t discord_id;
    std::string user_name;
    uint32_t purse;
    Stats user_stats;

public:

    User(uint64_t id, std::string user_name);

    uint64_t GetId();

    std::string GetUserName();

    std::string ToJson();

};