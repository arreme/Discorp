#include <user.hpp>

User::User(uint64_t id, std::string user_name)
: discord_id(id), user_name(user_name) 
{
    user_stats = Stats{0,0,0,0};
};

uint64_t User::GetId() 
{
    return discord_id;
}

std::string User::GetUserName() 
{
    return user_name;
}

std::string User::ToJson() 
{
    json j;
    j["discord_id"] = discord_id;
    j["user_name"] = user_name;
    return to_string(j);
}