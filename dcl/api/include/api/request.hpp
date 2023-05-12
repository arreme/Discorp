#pragma once
#include <ostream>
#include <core/pb/player.pb.h>
#include <dpp.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>
#include <db_handler/db_inventory.hpp>
#include <core/dcl_map.hpp>
#include <img/renderer.hpp>

class BaseRequest 
{
protected:
    bool m_user_created = false;
    PBUser m_user_db;
    db_handler::DBUserHandler m_user_handler{&m_user_db};
public:
    BaseRequest(uint64_t discord_id) 
    {
        m_user_db.set_discord_id(discord_id);
        m_user_created = m_user_handler.FindUserCurrentPlayer();
    };
};

class CreateGameRequest : public BaseRequest
{
private:
    const PBLocationID m_start_location = PBLocationID::MAIN_BASE;
    PBLocation m_location;
    db_handler::DBLocationHandler m_location_handler{&m_location};

public: 
    CreateGameRequest(uint64_t discord_id, std::string user_name)
    : BaseRequest(discord_id)
    {
        m_user_db.set_user_name(user_name);
    }

    void CreateGame(uint64_t guild_id);

    bool FillResponse(dpp::message &m);
};

class PrintMapRequest : public BaseRequest 
{
private:
    static const std::string S_MAP_BASE_PATH;
    static const std::string S_MAP_POST_PATH;
    static const std::string S_MAP_ZONE_ACCESS_LOCKED_PATH;

    PBLocation m_location_db;
    db_handler::DBLocationHandler m_location_handler{&m_location_db};
    const PBLocation *m_location_data;
    const int m_selected = 0;
public:
    PrintMapRequest(uint64_t discord_id, int selected);

    bool FillRequest(dpp::message &m);
};