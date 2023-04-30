#pragma once
#include <ostream>
#include <core/pb/player.pb.h>
#include <dpp.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>
#include <core/dcl_map.hpp>

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
    PBLocationID start_location = PBLocationID::MAIN_BASE;
public: 
    CreateGameRequest(uint64_t discord_id, std::string user_name)
    : BaseRequest(discord_id), m_user_name(user_name)
    {
        m_user.set_user_name(user_name);
    }

    void CreateGame(uint64 guild_id) 
    {
        //Create User and start a new game
        //Create user
        //Create map
        //Create inventory
        db::Transaction t;
        m_user.set_current_player_id(0);
        auto player = m_user.add_players();
        player->set_guild_id(guild_id);
        player->set_current_location(start_location);
        auto player_stats = player->mutable_stats();
        player_stats->set_max_health(20);
        player_stats->set_current_health(20);
        m_user_handler.InsertUser(&t);
        db_handler::DBLocationHandler::InsertNewLocation(m_user,DCLData::DCLMap::getInstance().GetLocation(start_location));

    }

    bool FillResponse(dpp::message &m) 
    {
        if (!m_user_created) 
        {
            return false;
        }

        //Print out map


        
        return true;
    }
};