#pragma once
#include <api/api_interactions.hpp>
#include <ostream>
#include <core/pb/player.pb.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>
#include <db_handler/db_inventory.hpp>

struct UserData 
{
    bool m_user_created = false;
    PBUser m_user_db;
    bool m_location_created = false;
    PBLocation m_location_db;
};

class BaseRequest 
{
protected:
    UserData m_data;
    db_handler::DBUserHandler m_user_handler{&m_data.m_user_db};
    db_handler::DBLocationHandler m_location_handler{&m_data.m_location_db};
public:
    BaseRequest(uint64_t discord_id) 
    {
        m_data.m_user_db.set_discord_id(discord_id);
        
        m_data.m_user_created = m_user_handler.FindUserCurrentPlayer();
        auto now = std::chrono::system_clock::now();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        m_data.m_user_db.mutable_last_online()->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
    };

    BaseRequest(UserData &&data)
    : m_data(std::move(data))
    {
        if (!m_data.m_user_created) {
            m_data.m_user_created = m_user_handler.FindUserCurrentPlayer();
        }
    }
};

class CreateGameRequest : public BaseRequest
{
private:
    const PBLocationID m_start_location = PBLocationID::MAIN_BASE;
    uint64_t m_guild_id;

    void CreateGame();

public: 
    CreateGameRequest(uint64_t discord_id, std::string user_name, uint64_t guild_id)
    : BaseRequest(discord_id), m_guild_id(guild_id)
    {
        m_data.m_user_db.set_user_name(user_name);
    }

    bool FillResponse(dpp::message &m);
};

class PrintMapRequest : public BaseRequest 
{
private:
    const int m_selected = -1;

    Renderer::BaseMapRenderer RenderMap(const PBLocation* location_data) 
    {
        if (m_selected >= 0)
        {
            switch (location_data->interactions(m_selected).types(0))
            {
            case PBInteractionType::POST: 
            {
                Renderer::PostMapRenderer renderer{m_selected};
                renderer.FillContents(m_data.m_user_db.players(0),*location_data, m_data.m_location_db);
                return renderer;
            }
            case PBInteractionType::ZONE_ACCESS:
            {
                auto zone_access_db = m_data.m_location_db.interactions(location_data->interactions(m_selected).database_id());
                int current_stat = location_data->interactions(m_selected).zone_access_info().unlock_info(zone_access_db.zone_access_info().unlock_level()).current_stat();
                Renderer::ZoneAccessRenderer renderer{m_selected,current_stat == 1};
                renderer.FillContents(m_data.m_user_db.players(0),*location_data, m_data.m_location_db);
                return renderer;
            } 
            default:
                break;
            }
        }

        Renderer::BaseMapRenderer renderer{};
        renderer.FillContents(m_data.m_user_db.players(0),*location_data, m_data.m_location_db);
        return renderer;
    }

public:
    PrintMapRequest(uint64_t discord_id, int selected);
    PrintMapRequest(UserData &&data, int selected = -1);
    bool FillRequest(dpp::message &m);


};