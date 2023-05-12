#include <api/request.hpp>

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

PrintMapRequest::PrintMapRequest(uint64_t discord_id, int selected) 
: BaseRequest(discord_id), m_selected(selected)
{
    if (!m_data.m_user_created) return;
    m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
};

PrintMapRequest::PrintMapRequest(UserData &&data, int selected)
: BaseRequest(std::move(data)) 
{
    if (!m_data.m_location_created) {
        m_data.m_location_created = m_location_handler.FindPlayerCurrentLocation(m_data.m_user_db);
    }
}

bool PrintMapRequest::FillRequest(dpp::message &m) 
{
    if (!m_data.m_user_created) return false;
    m.set_flags(dpp::m_ephemeral);

    const PBLocation *location_data = DCLData::DCLMap::getInstance().GetLocationData(m_data.m_user_db.players(m_data.m_user_db.current_player_id()).current_location());
    if (!location_data) 
    {
        m.set_content("Error! Please use command /repair to fix");
        return false;
    }
    

    Renderer::BaseMapRenderer renderer = RenderMap(location_data);
    int size = 0;
    m.add_file("map.png",std::string{renderer.RenderImage(&size).get(),static_cast<size_t>(size)});
    
    
    std::cout << m_selected << std::endl;
    if (m_selected != -1) 
    {
        auto buttons = dpp::component();
        for (auto const &type : location_data->interactions(m_selected).types())
        {
            buttons.add_component(dpp::component().set_label(PBInteractionType_Name(type)).set_id(std::to_string(m_data.m_user_db.discord_id()) +"_"+PBInteractionType_Name(type)));
        }
        m.add_component(
            buttons
        );
    }
    
    
    auto list = dpp::component().set_type(dpp::cot_selectmenu).
                    set_placeholder("Select Interaction").
                    set_id(std::to_string(m_data.m_user_db.discord_id()));
    int i = 0;
    for(auto const &interaction : location_data->interactions()) 
    {
        list.add_select_option(dpp::select_option(interaction.interaction_list_name(),std::to_string(i),interaction.description())).set_emoji("sandwich");
        i++;
    }
    m.add_component(
        dpp::component().add_component(list)
    );

    return true;
};

