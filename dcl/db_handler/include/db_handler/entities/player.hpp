#pragma once
#include <iostream>
#include <time.h>
#include <db_handler/game_constants.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

struct Stats
{
    uint32_t m_max_health = 20;
    uint32_t m_current_health = 20;
    int32_t m_strength = 0;
    int32_t m_defense = 0;
    int32_t m_precision = 0;
    int32_t m_speed = 0;
    int32_t m_luck = 0;
    bool m_is_dirty = false;

    Stats() = default;
    Stats(bsoncxx::document::element element);
    bsoncxx::document::value ToJson() const noexcept;
};

struct Skills
{
    uint32_t m_forage_lvl = 1;
    uint32_t m_forage_xp = 0;
    uint32_t m_mining_lvl = 1;
    uint32_t m_mining_xp = 0;
    uint32_t m_combat_lvl = 1;
    uint32_t m_combat_xp = 0;
    uint32_t m_athletics_lvl = 1;
    uint32_t m_athletics_xp = 0;

    Skills() = default;
    Skills(bsoncxx::document::element element);
    bsoncxx::document::value ToJson() const noexcept;
};

class Player 
{
private:
    uint64_t m_discord_id;
    int32_t m_player_id;
    g_enums::GameLocations m_current_loc;
    uint64_t m_guild_id = 0;
    Stats m_stats;
    Skills m_skills;
    //std::vector<Equipables> m_equipables;

public:
    Player(uint64_t discord_id, uint8_t player_id);

    Player(bsoncxx::document::view player);

    bsoncxx::document::value ToJson() const;

    g_enums::GameLocations GetLocation() const noexcept;

    int GetLocationInt() const noexcept;

    Skills* const GetSkills() noexcept;

    Stats* const GetStats() noexcept;
};