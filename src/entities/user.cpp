#include <user.hpp>

User::User(uint64_t id, std::string user_name)
: discord_id(id), user_name(user_name) 
{

};

uint64_t User::GetId() 
{
    return discord_id;
}

std::string User::GetUserName() 
{
    return user_name;
}