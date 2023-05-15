#include <api/request.hpp>

/**************************/
/*******BASE REQUEST*******/
/**************************/
BaseRequest::BaseRequest(uint64_t discord_id) 
{
    if (!m_data.m_user_created) {
        m_data.m_user_db.set_discord_id(discord_id);
        m_data.m_user_created = m_user_handler.FindUserCurrentPlayer();
    }
    
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    m_data.m_user_db.mutable_last_online()->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
};

/*************************/
/***CREATE GAME REQUEST***/
/*************************/
void CreateGameRequest::CreateGame() 
{
    db::Transaction t;
    m_data.m_user_db.set_current_player_id(0);
    auto last_online = m_data.m_user_db.mutable_last_online();
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    last_online->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
    auto player = m_data.m_user_db.add_players();
    player->set_guild_id(m_guild_id);
    player->set_current_location(m_start_location);
    auto player_stats = player->mutable_stats();
    player_stats->set_max_health(20);
    player_stats->set_current_health(20);
    m_user_handler.InsertUser(&t);
    m_data.m_location_db = DCLData::DCLMap::getInstance().GetLocation(m_start_location)->GetLocationDB();
    m_location_handler.InsertNewLocation(m_data.m_user_db,&t);
    db_handler::DBInventoryHandler::InitializeInventory(m_data.m_user_db,&t);
    t.ExecuteTransaction();
    m_data.m_user_created = (t.GetState() == db::OperationState::SUCCESS);
}

bool CreateGameRequest::FillResponse(dpp::message &m) 
{
    if (!m_data.m_user_created) 
    {
        CreateGame();
        return false;
    }
    PrintMapRequest print_map{std::move(m_data)};
    print_map.FillRequest(m);
    return true;
}

/*************************/
/****PRINT MAP REQUEST****/
/*************************/

PrintMapRequest::PrintMapRequest(uint64_t discord_id, int selected) 
: BaseRequest(discord_id), m_selected(selected)
{
    if (!m_data.m_user_created) return;
    m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
};

PrintMapRequest::PrintMapRequest(UserData &&data, int selected)
: BaseRequest(std::move(data)), m_selected(selected) 
{
    if (!m_data.m_location_created) {
        m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
    }
}

bool PrintMapRequest::FillRequest(dpp::message &m) 
{
    if (!m_data.m_user_created) return false;
    m.set_flags(dpp::m_ephemeral);

    const DCLData::DCLLocation *location_data = DCLData::DCLMap::getInstance().GetLocation(m_data.m_user_db.players(0).current_location());
    if (!location_data) 
    {
        m.set_content("Error! Please use command /repair to fix");
        return false;
    }
    
    Renderer::BaseMapRenderer renderer = RenderMap(location_data);
    int size = 0;
    m.add_file("map.png",std::string{renderer.RenderImage(&size).get(),static_cast<size_t>(size)});
    
    if (m_selected != -1) 
    {
        auto buttons = dpp::component();
        for (auto const &interaction : *location_data->GetInteraction(m_selected))
        {
            switch (interaction->GetType())
            {
            case PBInteractionType::POST:
                buttons.add_component(dpp::component().set_label("UPGRADE POST").set_id("upgrade_post::"+std::to_string(m_data.m_user_db.discord_id())+"::"+std::to_string(m_selected)).set_style(dpp::cos_success));
                buttons.add_component(dpp::component().set_label("COLLECT POST").set_id("collect_post::"+std::to_string(m_data.m_user_db.discord_id())+"::"+std::to_string(m_selected)).set_style(dpp::cos_primary));
                break;
            case PBInteractionType::ZONE_ACCESS:

                if (location_data->GetInteraction(m_selected)->TryGetZoneAccess()->IsUnlocked(m_data.m_location_db.interactions(location_data->GetInteraction(m_selected)->GetDatabaseId()).zone_access_info().unlock_level())) {
                    buttons.add_component(dpp::component().set_label("CHANGE LOCATION").set_id("go_to_location::"+std::to_string(m_data.m_user_db.discord_id())+"::"+std::to_string(m_selected)).set_style(dpp::cos_danger));
                } else {
                    buttons.add_component(dpp::component().set_label("UNLOCK LOCATION").set_id("unlock_zone::"+std::to_string(m_data.m_user_db.discord_id())+"::"+std::to_string(m_selected)).set_style(dpp::cos_danger));
                }
                break;
            case PBInteractionType::DIALOG:
                buttons.add_component(dpp::component().set_label("INTERACT").set_id("interact::"+std::to_string(m_data.m_user_db.discord_id())+"::"+std::to_string(m_selected)).set_style(dpp::cos_secondary));
                break;
            default:
                break;
            }
        }
        m.add_component(
            buttons
        );
    }
    
    auto list = dpp::component().set_type(dpp::cot_selectmenu).
                    set_placeholder("Select Interaction").
                    set_id("map_list::"+std::to_string(m_data.m_user_db.discord_id()));
    int i = 0;
    for(auto const &interaction : *location_data) 
    {
        list.add_select_option(dpp::select_option(interaction.GetListName(),std::to_string(i),interaction.GetDescription()));
        i++;
    }
    m.add_component(
        dpp::component().add_component(list)
    );

    m.add_component(
        dpp::component().add_component(
            dpp::component().set_label("INVENTORY").set_id("inventory::"+std::to_string(m_data.m_user_db.discord_id())).set_style(dpp::cos_secondary)
        ).add_component(
            dpp::component().set_label("PROFILE").set_id("profile::"+std::to_string(m_data.m_user_db.discord_id())).set_style(dpp::cos_secondary)
        )
    );

    return true;
};

Renderer::BaseMapRenderer PrintMapRequest::RenderMap(const DCLData::DCLLocation* location_data) 
{
    if (m_selected >= 0)
    {
        switch (location_data->GetInteraction(m_selected)->GetMainType()->GetType())
        {
        case PBInteractionType::POST: 
        {
            Renderer::PostMapRenderer renderer{m_selected};
            renderer.FillContents(m_data.m_user_db.players(0),*location_data, m_data.m_location_db);
            return renderer;
        }
        case PBInteractionType::ZONE_ACCESS:
        {
            auto zone_access_db = m_data.m_location_db.interactions(location_data->GetInteraction(m_selected)->GetDatabaseId());
            bool is_unlocked = location_data->GetInteraction(m_selected)->TryGetZoneAccess()->IsUnlocked(zone_access_db.zone_access_info().unlock_level());
            Renderer::ZoneAccessRenderer renderer{m_selected, is_unlocked};
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