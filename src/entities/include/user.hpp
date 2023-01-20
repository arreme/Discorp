#pragma once
#include <iostream>
#include <chrono>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

class User 
{
private:
    uint64_t m_discord_id = 1;
    std::string m_user_name;
    std::chrono::system_clock::time_point m_last_active = std::chrono::system_clock::now();
    uint8_t m_current_player = 1;
    uint8_t m_current_max_player = 1;
    bool m_has_dlc1 = false;
public:
    User(uint64_t id, std::string user_name);

    User(bsoncxx::document::view user);

    uint64_t GetId();

    std::string GetUserName();

    void ChangeCurrentPlayer(uint8_t current_player);

    void UpdateTime();

    bsoncxx::document::value ToJson();
};