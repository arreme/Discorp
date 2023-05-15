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
    PBUpgradeType m_type;
public:
    UpgradePostRequest(uint64_t discord_id, int selected, PBUpgradeType type)
    : BaseRequest(discord_id), m_selected(selected), m_type(type)
    {
        if (!m_data.m_location_created) {
            m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
        }
    };

    bool FillRequest(dpp::message &m) 
    {
        if (!m_data.m_user_created) return false;
        const DCLData::DCLInteraction *interaction_data = DCLData::DCLMap::getInstance().GetLocation(m_data.m_user_db.players(0).current_location())->GetInteraction(m_selected);
        auto const &post_info_db = m_data.m_location_db.interactions(interaction_data->GetDatabaseId()).post_info();
        const auto *post_info_data = interaction_data->TryGetPost();
        int current = 0;
        switch (m_type)
        {
        case PBUpgradeType::CAPACITY:
            current = post_info_db.capacity_upgrade();
            break;
        case PBUpgradeType::GEN_SECOND:
            current = post_info_db.gen_second_upgrade();
            break;
        case PBUpgradeType::FORTUNE:
            current = post_info_db.fortune_upgrade();
            break;
        default:
            break;
        }
        
        std::string result = "";
        for (auto const &item : post_info_data->GetUpgradeRequirements(m_type,current))
        {
            const std::string *item_name = DCLData::DCLItems::getInstance().GetItemName(item.item_id());
            result +=  *item_name+" - "+std::to_string(item.quantity())+"\n";
        }
        std::stringstream stream_current_stat;
        stream_current_stat << std::fixed << std::setprecision(2) << post_info_data->GetCurrentStat(m_type,current);
        std::string next_stat_str;
        float next_stat = post_info_data->GetCurrentStat(m_type,current+1);
        if (next_stat == -1)
        {
            next_stat_str = "---";
        } else 
        {
            std::stringstream stream_next_stat;
            stream_next_stat << std::fixed << std::setprecision(2) << next_stat;
            next_stat_str = stream_next_stat.str();
        }
        
        m.set_flags(dpp::m_ephemeral);
        dpp::embed embed = dpp::embed().
            set_color(dpp::colors::forest_green).
            set_title("["+interaction_data->GetInteractionName()+"] Upgrade Menu").
            set_description("You have now selected: "+PBUpgradeType_Name(m_type)).
            add_field(
                "Current Level Stat",
                stream_current_stat.str(),
                true
            ).add_field(
                "Next Level Stat",
                next_stat_str,
                true
            ).add_field(
                "Upgrade Requirements",
                result
            );

        m.add_embed(embed);
        auto list = dpp::component().set_type(dpp::cot_selectmenu).
                    set_placeholder("Select Upgrade").
                    set_id("upgrade_post_list::"+std::to_string(m_data.m_user_db.discord_id())+"::"+std::to_string(m_selected)).
                    add_select_option(dpp::select_option("[CAPACITY]",std::to_string(PBUpgradeType::CAPACITY),"Do you want to save more resources?")).
                    add_select_option(dpp::select_option("[GENERATION]",std::to_string(PBUpgradeType::GEN_SECOND),"To Generate even more resources per second?")).
                    add_select_option(dpp::select_option("[FORTUNE]",std::to_string(PBUpgradeType::FORTUNE),"If you are not feeling lucky, improve this!"));
        m.add_component(
            dpp::component().add_component(list)
        );
        return true;
    }
};