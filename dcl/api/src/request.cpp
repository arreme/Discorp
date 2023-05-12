#include <api/request.hpp>

void CreateGameRequest::CreateGame(uint64_t guild_id) 
{
    db::Transaction t;
    m_user_db.set_current_player_id(0);
    auto last_online = m_user_db.mutable_last_online();
    auto now = std::chrono::system_clock::now();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    last_online->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
    auto player = m_user_db.add_players();
    player->set_guild_id(guild_id);
    player->set_current_location(m_start_location);
    auto player_stats = player->mutable_stats();
    player_stats->set_max_health(20);
    player_stats->set_current_health(20);
    m_user_handler.InsertUser(&t);
    m_location = DCLData::DCLMap::getInstance().GetLocation(m_start_location)->GetLocationDB();
    m_location_handler.InsertNewLocation(m_user_db,&t);
    db_handler::DBInventoryHandler::InitializeInventory(m_user_db,&t);
    t.ExecuteTransaction();
    m_user_created = (t.GetState() == db::OperationState::SUCCESS);
}

bool CreateGameRequest::FillResponse(dpp::message &m) 
{
    if (!m_user_created) 
    {
        return false;
    }
    //GetLocationInfo
    m_location_handler.FindPlayerCurrentLocation(m_user_db);
    //Print out map
    m.set_flags(dpp::m_ephemeral);
    Renderer::BaseMapRenderer map_renderer{};
    const PBLocation *const m_location_data = DCLData::DCLMap::getInstance().GetLocationData(m_user_db.players(m_user_db.current_player_id()).current_location());
    if (!m_location_data) 
    {
        std::cout << "Invalid location, move player to MAIN_BASE" << std::endl;
        m.set_content("Error! Please use command /repair to fix");
        return true;
    }
    map_renderer.FillContents(m_user_db.players(0),*m_location_data, m_location);
    int size = 0;
    m.add_file("map.png",std::string{map_renderer.RenderImage(&size).get(),static_cast<size_t>(size)});
    auto list = dpp::component().set_type(dpp::cot_selectmenu).
                set_placeholder("Select Interaction").
                set_id(std::to_string(m_user_db.discord_id()));
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

const std::string PrintMapRequest::S_MAP_BASE_PATH = "";
const std::string PrintMapRequest::S_MAP_POST_PATH = "";
const std::string PrintMapRequest::S_MAP_ZONE_ACCESS_LOCKED_PATH = "";

PrintMapRequest::PrintMapRequest(uint64_t discord_id, int selected) 
: BaseRequest(discord_id), m_selected(selected)
{
    if (!m_user_created) return;
    m_location_data = DCLData::DCLMap::getInstance().GetLocationData(m_user_db.players(m_user_db.current_player_id()).current_location());
    m_location_handler.FindPlayerCurrentLocation(m_user_db);
};

bool PrintMapRequest::FillRequest(dpp::message &m) 
{
    if (!m_user_created) return false;

    m.set_flags(dpp::m_ephemeral);
    Renderer::PostMapRenderer map_renderer{m_selected};
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
                set_id(std::to_string(m_user_db.discord_id()));
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
};