#pragma once
#include <img/renderer.hpp>
#include <core/dcl_map.hpp>
#include <core/dcl_items.hpp>
#include <dpp.h>
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
    std::vector<PBItemData> m_new_items;
    Renderer::BaseMapRenderer RenderMap(const DCLData::DCLLocation* location_data);

public:
    PrintMapRequest(uint64_t discord_id, int selected);
    PrintMapRequest(UserData &&data, int selected);
    PrintMapRequest(UserData &&data, int selected, std::vector<PBItemData> new_items);
    bool FillRequest(dpp::message &m);
};

class BuyableRequest : public BaseRequest 
{
private:
    std::vector<PBItemData> m_items_db;
    db_handler::DBInventoryHandler m_items_handler{&m_items_db};
    PBItemType m_item_type;
public:
    BuyableRequest(uint64_t discord_id, PBItemType item_type) 
    : BaseRequest(discord_id), m_item_type(item_type)
    {}

    bool Buy(const google::protobuf::RepeatedPtrField<PBItemData> item_data) 
    {
        for (auto const &item_req : item_data)
        {
            PBItemData temp;
            temp.set_item_id(item_req.item_id());
            m_items_db.push_back(temp);
        }
        m_items_handler.GetItems(m_data.m_user_db.discord_id(),m_data.m_user_db.current_player_id(),PBItemType_Name(m_item_type));
        
        if (m_items_db.size() != item_data.size() ) return false;
        
        for (int i = 0; i < item_data.size(); i++)
        {
            if (m_items_db.at(i).item_id() == item_data.at(i).item_id())
            {
                if (m_items_db.at(i).quantity() >= item_data.at(i).quantity())
                {
                    m_items_db.at(i).set_quantity( m_items_db.at(i).quantity() - item_data.at(i).quantity() );
                } else 
                {
                    return false;
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
    int32_t m_type;
public:
    UpgradePostRequest(uint64_t discord_id, int selected, int32_t type)
    : BuyableRequest(discord_id, PBItemType::RESOURCES), m_selected(selected), m_type(type)
    {
        if (!m_data.m_location_created) {
            m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
        }
    };

    bool FillRequest(dpp::message &m);

    bool ConfirmRequest();
};

class UnlockLocationRequest : public BuyableRequest
{
private:
    int m_selected;
public:
    UnlockLocationRequest(uint64_t discord_id, int selected)
    : BuyableRequest(discord_id, PBItemType::RESOURCES), m_selected(selected)
    {
        if (!m_data.m_location_created) {
            m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
        }
    };

    bool FillRequest(dpp::message &m);

    bool ConfirmRequest();
};

class CollectPostRequest : public BaseRequest
{
private:
    int m_selected;
    std::vector<PBItemData> m_items_collected;
public:
    CollectPostRequest(uint64_t discord_id, int selected)
    : BaseRequest(discord_id), m_selected(selected)
    {
        if (!m_data.m_location_created) {
            m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
        }
    };
    
    bool ConfirmRequest();

    bool FillRequest(dpp::message &m);
};

class GoToLocationRequest : public BaseRequest
{
private:
    int m_selected;
public:
    GoToLocationRequest(uint64_t discord_id, int selected)
    : BaseRequest(discord_id), m_selected(selected)
    {};

    bool ConfirmRequest();
};

class PrintInventoryRequest : public BaseRequest
{
private:
    int m_item_type;
    int m_page;
    std::vector<PBItemData> m_item_db;
    db_handler::DBInventoryHandler m_item_handler{&m_item_db};
public:
    PrintInventoryRequest(uint64_t discord_id, int item_type, int page)
    : BaseRequest(discord_id), m_item_type(item_type), m_page(page)
    {}

    bool FillRequest(dpp::message &m);
};