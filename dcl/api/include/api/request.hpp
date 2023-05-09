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
    int m_selected = 0;
public:
    PrintMapRequest(uint64_t discord_id, int selected) 
    : BaseRequest(discord_id)
    {
        if (!m_user_created) return;

        m_selected = selected;
        m_location_data = DCLData::DCLMap::getInstance().GetLocationData(m_user_db.players(m_user_db.current_player_id()).current_location());
        m_location_handler.FindPlayerCurrentLocation(m_user_db);
    };

    bool FillRequest(dpp::message &m) 
    {
        m.set_flags(dpp::m_ephemeral);
        Renderer::BaseMapRenderer map_renderer{};
        if (!m_location_data) 
        {
            m.set_content("Error! Please use command /repair to fix");
            return false;
        }
        map_renderer.FillContents(m_user_db.players(0),*m_location_data, m_location_db);
        int size = 0;
        m.add_file("map.png",std::string{map_renderer.RenderImage(&size).get(),static_cast<size_t>(size)});
        auto list = dpp::component().set_type(dpp::cot_selectmenu).
                    set_placeholder("Select Interaction").
                    set_id("myselid");
        int i = 0;
        for(auto const &interaction : m_location_data->interactions()) 
        {
            list.add_select_option(dpp::select_option(interaction.interaction_list_name(),std::to_string(i),interaction.description())).set_emoji("sandwich");
            i++;
        }
        m.add_component(
            dpp::component().add_component(list)
        );
    };
};