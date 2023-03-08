#pragma once
#include <iostream>
#include <chrono>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

#define MAX_USERS_NO_DLC1 4
#define MAX_USERS_DLC1 10

class User 
{
private:
    uint64_t m_discord_id;
    std::string m_user_name;
    std::chrono::system_clock::time_point m_last_active = std::chrono::system_clock::now();
    int32_t m_current_player = 1;
    bool m_has_dlc1 = false;
public:
    User(uint64_t id, std::string user_name);

    User(bsoncxx::document::view user);

    uint64_t GetId() const noexcept;

    std::string GetUserName() const noexcept;

    /**
     * @return true if was able to change current player.
     * false otherwise.
    */
    bool SetCurrentPlayer(int32_t current_player) noexcept;

    void UpdateTime() noexcept;

    bsoncxx::document::value ToJson() const noexcept;

    int32_t GetCurrentPlayer() const noexcept;

    bool HasDLC() const noexcept;
};