#include <user.hpp>

User::User(uint64_t id, std::string user_name)
: m_discord_id(id), m_user_name(user_name) 
{
    m_current_player = 0;
    m_current_max_player = 0;
    m_last_active = time(nullptr);
};

User::User(std::string user) 
{
    json j = json::parse(user);
    m_discord_id = j["discord_id"];
    m_user_name = j["user_name"];
    m_last_active = j["last_active"];
    m_current_player = j["current_player"];
    m_current_max_player = j["current_max_player"];
    m_has_dlc1 = j["has_dlc1"];
}

uint64_t User::GetId() 
{
    return m_discord_id;
}

void User::UpdateTime() 
{
    m_last_active = time(nullptr);
}

void User::ChangeCurrentPlayer(uint8_t current_player) 
{
    m_current_player = current_player;
}

std::string User::GetUserName() 
{
    return m_user_name;
}



std::string User::ToJson() 
{
    json j;
    j["discord_id"] = m_discord_id;
    j["user_name"] = m_user_name;
    j["last_active"] = m_last_active;
    j["current_player"] = m_current_player;
    j["current_max_player"] = m_current_max_player;
    j["has_dlc1"] = m_has_dlc1;
    return to_string(j);
}