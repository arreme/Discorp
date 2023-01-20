#include <user.hpp>

using bsoncxx::builder::basic::kvp;

User::User(uint64_t id, std::string user_name)
: m_discord_id(id), m_user_name(user_name) 
{};

User::User(bsoncxx::document::view user) 
{
    m_discord_id = user["discord_id"].get_int64();
    m_user_name = user["user_name"].get_utf8().value.to_string();
    m_last_active = user["last_active"].get_date();
    m_current_player = user["current_player"].get_int32();
    m_current_max_player = user["current_max_player"].get_int32();
    m_has_dlc1 = user["has_dlc1"].get_bool();
}

uint64_t User::GetId() 
{
    return m_discord_id;
}

void User::UpdateTime() 
{
    m_last_active = std::chrono::system_clock::now();
}

void User::ChangeCurrentPlayer(uint8_t current_player) 
{
    m_current_player = current_player;
}

std::string User::GetUserName() 
{
    return m_user_name;
}

bsoncxx::document::value User::ToJson() 
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("discord_id",bsoncxx::types::b_int64(m_discord_id)));
    doc.append(kvp("user_name",bsoncxx::types::b_utf8(m_user_name)));
    doc.append(kvp("last_active",bsoncxx::types::b_date(m_last_active)));
    doc.append(kvp("current_player",bsoncxx::types::b_int32(m_current_player)));
    doc.append(kvp("current_max_player",bsoncxx::types::b_int32(m_current_max_player)));
    doc.append(kvp("has_dlc1",bsoncxx::types::b_bool(m_has_dlc1)));
    return doc.extract();
}