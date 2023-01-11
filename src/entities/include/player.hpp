#pragma once
#include <iostream>
#include <json.hpp>
#include <time.h>
#include <game_constants.hpp>

using json = nlohmann::json;

struct Stats
{
    uint32_t m_max_health;
    uint32_t m_current_health;
    int32_t m_strength;
    int32_t m_defense;
    int32_t precision;
    bool m_is_dirty;
};

struct Skills
{
    uint16_t m_forage_lvl;
    uint32_t m_forage_xp;
    uint16_t m_mining_lvl;
    uint32_t m_mining_xp;
    uint16_t m_combat_lvl;
    uint32_t m_combat_xp;
};


class Player 
{
private:
    uint64_t m_discord_id;
    std::string m_player_name;
    uint8_t m_player_id;
    g_enums::GameLocations m_current_location;
    uint64_t m_guild_id;
    Stats m_player_stats;
    Skills m_player_skills;
public:
    Player(uint64_t discord_id, std::string player_name, uint8_t player_id);

    Player(std::string player);

    std::string ToJson();
};