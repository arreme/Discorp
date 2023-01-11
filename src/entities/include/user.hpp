#pragma once
#include <iostream>
#include <json.hpp>
#include <time.h>

using json = nlohmann::json;

class User 
{
private:
    uint64_t m_discord_id;
    std::string m_user_name;
    time_t m_last_active;
    uint8_t m_current_player;
    uint8_t m_current_max_player;
    bool m_has_dlc1;
public:
    User(uint64_t id, std::string user_name);

    User::User(std::string user);

    uint64_t GetId();

    std::string GetUserName();

    void ChangeCurrentPlayer(uint8_t current_player);

    void UpdateTime();

    std::string ToJson();
};