#pragma once
#include <api/api_interactions.hpp>
#include <ostream>
#include <core/pb/player.pb.h>
#include <db_handler/db_user.hpp>
#include <db_handler/db_location.hpp>
#include <db_handler/db_inventory.hpp>
#include <core/dcl_interactions.hpp>

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
    BaseRequest(uint64_t discord_id);

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

    Renderer::BaseMapRenderer RenderMap(const DCLData::DCLLocation* location_data);

public:
    PrintMapRequest(uint64_t discord_id, int selected);
    PrintMapRequest(UserData &&data, int selected = -1);
    bool FillRequest(dpp::message &m);
};

class UpgradePostRequest : public BaseRequest
{
private:
    int m_selected;
public:
    UpgradePostRequest(uint64_t discord_id, int selected)
    : BaseRequest(discord_id), m_selected(selected)
    {
        if (!m_data.m_location_created) {
            m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
        }
    };

    bool FillRequest(dpp::message &m) 
    {
        const DCLData::DCLInteraction *location_data = DCLData::DCLMap::getInstance().GetLocation(m_data.m_user_db.players(0).current_location())->GetInteraction(m_selected);

        m.set_flags(dpp::m_ephemeral);
        dpp::embed embed = dpp::embed().
            set_color(dpp::colors::forest_green).
            set_title("POST UPGRADE MENU").
            set_description("Select the field you want to upgrade:").
            set_author("Arreme","dwa","dwa").
            add_field(
                "Current Selected",
                ""
            ).add_field(
                "Requirements",
                "",
                true
            ).add_field(
                "You have",
                "",
                true
            );

        m.add_embed(embed);
        return true;
    }
};