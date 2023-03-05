#include <entities/user.hpp>

using bsoncxx::builder::basic::kvp;

User::User(uint64_t id, std::string user_name)
: m_discord_id(id), m_user_name(user_name) 
{};

User::User(bsoncxx::document::view user) 
{
    m_discord_id = static_cast<uint64_t>(user["discord_id"].get_int64());
    m_user_name = user["user_name"].get_string();
    m_last_active = user["last_active"].get_date();
    m_current_player = user["current_player"].get_int32();
    m_has_dlc1 = user["has_dlc1"].get_bool();
}

uint64_t User::GetId() const noexcept
{
    return m_discord_id;
}

void User::UpdateTime() noexcept
{
    m_last_active = std::chrono::system_clock::now();
}

bool User::ChangeCurrentPlayer(int32_t current_player) noexcept
{
    if (m_current_player > (m_has_dlc1 ? MAX_USERS_DLC1 : MAX_USERS_NO_DLC1))
    {
        return false;
    }
    m_current_player = current_player;
    return true;
}

std::string User::GetUserName() const noexcept
{
    return m_user_name;
}

bsoncxx::document::value User::ToJson() const noexcept
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("discord_id",bsoncxx::types::b_int64{static_cast<int64_t>(m_discord_id)}));
    doc.append(kvp("user_name",bsoncxx::types::b_string(m_user_name)));
    doc.append(kvp("last_active",bsoncxx::types::b_date(m_last_active)));
    doc.append(kvp("current_player",bsoncxx::types::b_int32{m_current_player}));
    doc.append(kvp("has_dlc1",bsoncxx::types::b_bool{m_has_dlc1}));
    return doc.extract();
}

uint8_t User::GetCurrentPlayer() const noexcept
{
    return m_current_player;
}

bool User::HasDLC() const noexcept
{
    return m_has_dlc1;
}