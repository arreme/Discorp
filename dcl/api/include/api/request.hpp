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
    PBUser m_user;
    db_handler::DBUserHandler m_user_handler{&m_user};
public:
    BaseRequest(uint64_t discord_id) 
    {
        m_user.set_discord_id(discord_id);
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
        m_user.set_user_name(user_name);
    }

    void CreateGame(uint64_t guild_id) 
    {
        db::Transaction t;
        m_user.set_current_player_id(0);
        auto last_online = m_user.mutable_last_online();
        auto now = std::chrono::system_clock::now();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        last_online->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
        auto player = m_user.add_players();
        player->set_guild_id(guild_id);
        player->set_current_location(m_start_location);
        auto player_stats = player->mutable_stats();
        player_stats->set_max_health(20);
        player_stats->set_current_health(20);
        m_user_handler.InsertUser(&t);
        m_location = DCLData::DCLMap::getInstance().GetLocation(m_start_location)->GetLocationDB();
        m_location_handler.InsertNewLocation(m_user,&t);
        db_handler::DBInventoryHandler::InitializeInventory(m_user,&t);
        t.ExecuteTransaction();
        m_user_created = (t.GetState() == db::OperationState::SUCCESS);
    }

    bool FillResponse(dpp::message &m) 
    {
        if (!m_user_created) 
        {
            return false;
        }
        //GetLocationInfo
        m_location_handler.FindPlayerCurrentLocation(m_user);
        //Print out map
        m.set_flags(dpp::m_ephemeral);
        Renderer::BaseMapRenderer map_renderer{};
        const PBLocation *const m_location_data = DCLData::DCLMap::getInstance().GetLocationData(m_user.players(m_user.current_player_id()).current_location());
        if (!m_location_data) 
        {
            std::cout << "Invalid location, move player to MAIN_BASE" << std::endl;
            m.set_content("Error! Please use command /repair to fix");
            return true;
        }
        map_renderer.FillContents(m_user.players(0),*m_location_data, m_location);
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
        return true;
    }
};