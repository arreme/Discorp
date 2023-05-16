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
    std::vector<PBItemData> m_item_resources_db;
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

class BuyableRequest : public BaseRequest 
{
private:
    db_handler::DBInventoryHandler m_items_handler{&m_data.m_item_resources_db};
    PBItemType m_item_type;
public:
    BuyableRequest(uint64_t discord_id, PBItemType item_type) 
    : BaseRequest(discord_id), m_item_type(item_type)
    {
        m_items_handler.GetItems(m_data.m_user_db.discord_id(),m_data.m_user_db.current_player_id(),PBItemType_Name(m_item_type));
    }

    bool Buy(const google::protobuf::RepeatedPtrField<PBItemData> item_data) 
    {
        if (m_data.m_item_resources_db.size() != item_data.size() ) return false;
        
        for (auto const &item_req : item_data)
        {
            for (auto item_db : m_data.m_item_resources_db)
            {
                if ((item_db.item_id() == item_req.item_id()) && item_db.quantity() < item_req.item_id()) 
                {
                    return false;
                } else if(item_db.item_id() == item_req.item_id()) 
                {
                    std::cout << "Can Buy!" << std::endl;
                    item_db.set_quantity( item_db.quantity() - item_req.quantity() );
                    //You can stop looking as item already has more quantity
                    break;
                }
            }
        }
        m_items_handler.UpdateItems(m_data.m_user_db.discord_id(),m_data.m_user_db.current_player_id());
        return true;
    }
};

class UpgradePostRequest : public BuyableRequest
{
private:
    int m_selected;
    PBUpgradeType m_type;
public:
    UpgradePostRequest(uint64_t discord_id, int selected, PBUpgradeType type)
    : BuyableRequest(discord_id, PBItemType::RESOURCES), m_selected(selected), m_type(type)
    {
        if (!m_data.m_location_created) {
            m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
        }
    };

    bool FillRequest(dpp::message &m);

    bool ConfirmRequest() 
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
        if (Buy(post_info_data->GetUpgradeRequirements(m_type,current))) 
        {
            std::cout << "YEah!" << std::endl;
            return true;
        }
        return false;
    }
};