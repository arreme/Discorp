#pragma once
#include <ostream>
#include <core/pb/player.pb.h>
#include <dpp.h>

class BaseRequest 
{
protected:
    bool user_created = false;
    PBUser m_user;
public:
    BaseRequest(uint64_t discord_id) 
    {
        m_user.set_discord_id(discord_id);
        //Check If user already registerered
    };
};

class CreateGameRequest : public BaseRequest
{
private:
    uint64_t m_discord_id;
    std::string m_user_name;
public: 
    CreateGameRequest(uint64_t discord_id, std::string user_name)
    : BaseRequest(discord_id), m_user_name(user_name)
    {
        m_user.set_user_name(user_name);
    }

    bool FillResponse(dpp::message &m) 
    {

    }
};