#include <db_handler/entities/player.hpp>

using bsoncxx::builder::basic::kvp;
using namespace bsoncxx::types;

/**
 * Only should be called once for first initializing a player. After 
 * that Players should be initialized by using the database
*/
Player::Player(uint64_t discord_id, int32_t player_id, int32_t init_loc) 
: m_discord_id(discord_id), m_player_id(player_id), m_current_loc(init_loc)
{}

Player::Player(bsoncxx::document::view player)
{
    m_discord_id = static_cast<uint64_t>(player["discord_id"].get_int64());
    m_player_id = player["player_id"].get_int32();
    m_guild_id = static_cast<uint64_t>(player["guild_id"].get_int64());
    m_current_loc = player["current_loc"].get_int32().value;
    m_stats = {player["stats"]};
    m_skills = {player["skills"]};
}

Stats::Stats(bsoncxx::document::element element) : 
m_max_health(static_cast<uint32_t>(element["max_health"].get_int32())),
m_current_health(static_cast<uint32_t>(element["current_health"].get_int32())),
m_strength(element["strength"].get_int32()),
m_defense(element["defense"].get_int32()),
m_precision(element["precision"].get_int32()),
m_speed(element["speed"].get_int32()),
m_luck(element["luck"].get_int32()),
m_is_dirty(element["is_dirty"].get_bool())
{}

Skills::Skills(bsoncxx::document::element element) : 
m_forage_lvl(static_cast<uint32_t>(element["forage_lvl"].get_int32())),
m_forage_xp(static_cast<uint32_t>(element["forage_xp"].get_int32())),
m_mining_lvl(static_cast<uint32_t>(element["mining_lvl"].get_int32())),
m_mining_xp(static_cast<uint32_t>(element["mining_xp"].get_int32())),
m_combat_lvl(static_cast<uint32_t>(element["combat_lvl"].get_int32())),
m_combat_xp(static_cast<uint32_t>(element["combat_xp"].get_int32())),
m_athletics_lvl(static_cast<uint32_t>(element["athletics_lvl"].get_int32())),
m_athletics_xp(static_cast<uint32_t>(element["athletics_xp"].get_int32()))
{}

bsoncxx::document::value Player::ToJson() const
{
    auto doc = bsoncxx::builder::basic::document{};

    doc.append(kvp("discord_id",bsoncxx::types::b_int64{static_cast<int64_t>(m_discord_id)}));
    doc.append(kvp("player_id",bsoncxx::types::b_int32{m_player_id}));
    doc.append(kvp("current_loc",bsoncxx::types::b_int32{m_current_loc}));
    doc.append(kvp("guild_id",bsoncxx::types::b_int64{static_cast<int64_t>(m_guild_id)}));
    
    doc.append(kvp("stats",m_stats.ToJson()));
    doc.append(kvp("skills",m_skills.ToJson()));

    return doc.extract();
}

bsoncxx::document::value Stats::ToJson() const noexcept
{
    auto stats = bsoncxx::builder::basic::document{};

    stats.append(kvp("max_health",b_int32{static_cast<int32_t>(m_max_health)}));
    stats.append(kvp("current_health",b_int32{static_cast<int32_t>(m_current_health)}));
    stats.append(kvp("strength",b_int32{m_strength}));
    stats.append(kvp("defense",b_int32{m_defense}));
    stats.append(kvp("precision",b_int32{m_precision}));
    stats.append(kvp("speed",b_int32{m_speed}));
    stats.append(kvp("luck",b_int32{m_luck}));
    stats.append(kvp("is_dirty",b_bool{m_is_dirty}));

    return stats.extract();
}

bsoncxx::document::value Skills::ToJson() const noexcept 
{
    auto skills = bsoncxx::builder::basic::document{};

    skills.append(kvp("forage_lvl",b_int32{static_cast<int32_t>(m_forage_lvl)}));
    skills.append(kvp("forage_xp",b_int32{static_cast<int32_t>(m_forage_xp)}));
    skills.append(kvp("mining_lvl",b_int32{static_cast<int32_t>(m_mining_lvl)}));
    skills.append(kvp("mining_xp",b_int32{static_cast<int32_t>(m_mining_xp)}));
    skills.append(kvp("combat_lvl",b_int32{static_cast<int32_t>(m_combat_lvl)}));
    skills.append(kvp("combat_xp",b_int32{static_cast<int32_t>(m_combat_xp)}));
    skills.append(kvp("athletics_lvl",b_int32{static_cast<int32_t>(m_athletics_lvl)}));
    skills.append(kvp("athletics_xp",b_int32{static_cast<int32_t>(m_athletics_xp)}));

    return skills.extract();
}

int Player::GetLocation() const noexcept
{
    return m_current_loc;
}

Skills* const Player::GetSkills() noexcept
{
    return &m_skills;
}

Stats* const Player::GetStats() noexcept
{
    return &m_stats;
}

uint64_t Player::GetId() 
{
    return m_discord_id;
}

int32_t Player::GetPlayerId() 
{
    return m_player_id;
}