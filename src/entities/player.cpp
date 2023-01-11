#include <player.hpp>

Player::Player(uint64_t discord_id, std::string player_name, uint8_t player_id) 
: m_discord_id(discord_id), m_player_name(player_name), m_player_id(player_id)
{
    m_current_location = g_enums::GameLocations::MAIN_BASE;
    uint64_t m_guild_id = 0;
    m_player_stats = {20,20,1,1,1,false};
    m_player_skills = {1,0,1,0,1,0};
}

Player::Player(std::string player)
{
    json j = json::parse(player);
    m_discord_id = j["discord_id"];
    m_player_name = j["player_name"];
    m_player_id = j["player_id"];
    m_current_location = static_cast<g_enums::GameLocations>(j["current_location"]);
    
    m_player_stats = 
    {
        j["max_health"],
        j["current_health"],
        j["strength"],
        j["defense"],
        j["precision"],
        j["is_dirty"]
    };

    m_player_skills =
    {
        j["forage_lvl"],
        j["forage_xp"],
        j["mining_lvl"],
        j["mining_xp"],
        j["combat_lvl"],
        j["combat_xp"],
    };

}

std::string Player::ToJson() 
{
    json j;

    j["discord_id"] = m_discord_id;
    j["player_name"] = m_player_name;
    j["player_id"] = m_player_id;
    j["current_location"] = static_cast<int>(m_current_location);
    j["max_health"] = m_player_stats.m_max_health;
    j["current_health"] = m_player_stats.m_current_health;
    j["strength"] = m_player_stats.m_strength;
    j["defense"] = m_player_stats.m_defense;
    j["precision"] = m_player_stats.precision;
    j["is_dirty"] = m_player_stats.m_is_dirty;
    j["forage_lvl"] = m_player_skills.m_forage_lvl;
    j["forage_xp"] = m_player_skills.m_forage_xp;
    j["mining_lvl"] = m_player_skills.m_mining_lvl;
    j["mining_xp"] = m_player_skills.m_mining_xp;
    j["combat_lvl"] = m_player_skills.m_combat_lvl;
    j["combat_xp"] = m_player_skills.m_combat_xp;
    
    return to_string(j);
}