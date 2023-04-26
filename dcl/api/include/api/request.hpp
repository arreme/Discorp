#pragma once
#include <ostream>
#include <core/pb/player.pb.h>
#include <dpp.h>
#include <db_handler/db_user.hpp>

class BaseRequest 
{
protected:
    bool m_user_created = false;
    PBUser m_user;
    db_handler::DBUserHandler m_user_handler{&m_user};
public:
    BaseRequest(uint64_t discord_id) 
    {
        m_user.set_discord_id(discord_id);
        //Check If user already registerered
        m_user_created = m_user_handler.FindUserCurrentPlayer();
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
        if (!m_user_created) 
        {
            m.set_content("You are not registered inside the database!");
            return false;
        }

        db::Transaction t;
        return true;
    }
};