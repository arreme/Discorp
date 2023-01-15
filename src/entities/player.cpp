#include <player.hpp>

using bsoncxx::builder::basic::kvp;

Player::Player(uint64_t discord_id, std::string player_name, uint8_t player_id) 
: m_discord_id(discord_id), m_player_name(player_name), m_player_id(player_id)
{
    m_current_location = g_enums::GameLocations::MAIN_BASE;
    uint64_t m_guild_id = 0;
    m_player_stats = {20,20,1,1,1,false};
    m_player_skills = {1,0,1,0,1,0};
}

Player::Player(bsoncxx::document::view player)
{
    m_discord_id = player["discord_id"].get_int64();
    m_player_name = player["player_name"].get_utf8().value.to_string();
    m_player_id = player["player_id"].get_int32();
    m_current_location = static_cast<g_enums::GameLocations>(player["current_location"].get_int32().value);
    
    m_player_stats = 
    {
        player["stats"]["max_health"].get_int32(),
        player["stats"]["current_health"].get_int32(),
        player["stats"]["strength"].get_int32(),
        player["stats"]["defense"].get_int32(),
        player["stats"]["precision"].get_int32(),
        player["stats"]["is_dirty"].get_bool(),
    };

    m_player_skills =
    {
        player["skills"]["forage_lvl"].get_int32(),
        player["skills"]["forage_xp"].get_int32(),
        player["skills"]["mining_lvl"].get_int32(),
        player["skills"]["mining_xp"].get_int32(),
        player["skills"]["combat_lvl"].get_int32(),
        player["skills"]["combat_xp"].get_int32(),
    };

}

bsoncxx::document::value Player::ToJson() 
{
    auto doc = bsoncxx::builder::basic::document{};
    doc.append(kvp("discord_id",bsoncxx::types::b_int64(m_discord_id)));
    doc.append(kvp("player_name",bsoncxx::types::b_utf8(m_player_name)));
    doc.append(kvp("player_id",bsoncxx::types::b_int32(m_player_id)));
    doc.append(kvp("current_location",bsoncxx::types::b_int32(static_cast<int>(m_current_location))));
    auto stats = bsoncxx::builder::basic::document{};
    stats.append(kvp("max_health",bsoncxx::types::b_int32(m_player_stats.m_max_health)));
    stats.append(kvp("current_health",bsoncxx::types::b_int32(m_player_stats.m_current_health)));
    stats.append(kvp("strength",bsoncxx::types::b_int32(m_player_stats.m_strength)));
    stats.append(kvp("defense",bsoncxx::types::b_int32(m_player_stats.m_defense)));
    stats.append(kvp("precision",bsoncxx::types::b_int32(m_player_stats.precision)));
    stats.append(kvp("is_dirty",bsoncxx::types::b_bool(m_player_stats.m_is_dirty)));
    doc.append(kvp("stats",stats));
    auto skills = bsoncxx::builder::basic::document{};
    skills.append(kvp("forage_lvl",bsoncxx::types::b_int32(m_player_skills.m_forage_lvl)));
    skills.append(kvp("forage_xp",bsoncxx::types::b_int32(m_player_skills.m_forage_xp)));
    skills.append(kvp("mining_lvl",bsoncxx::types::b_int32(m_player_skills.m_mining_lvl)));
    skills.append(kvp("mining_xp",bsoncxx::types::b_int32(m_player_skills.m_mining_xp)));
    skills.append(kvp("combat_lvl",bsoncxx::types::b_int32(m_player_skills.m_combat_lvl)));
    skills.append(kvp("mining_xp",bsoncxx::types::b_int32(m_player_skills.m_combat_xp)));
    doc.append(kvp("skills",skills));
    return doc.extract();
}