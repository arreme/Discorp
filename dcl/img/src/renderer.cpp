#include <img/renderer.hpp>

Renderer::ImageRenderer::ImageRenderer(const std::string &base_image) 
{
    std::ifstream in{base_image, std::ios::binary};
    m_image = GD::Image{in,GD::Png_tag{}};
}

std::unique_ptr<char, void(*)(char*)> Renderer::ImageRenderer::RenderImage(int *size) 
{
    return std::unique_ptr<char, void(*)(char*)>{(char *) m_image.Png(size), Renderer::FreePointer};
}

void Renderer::FreePointer(char * data) 
{
    gdFree((void *)data);
}

bool Renderer::BaseMapRenderer::FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db) 
{
    if (!m_image.good()) return false;
#pragma region fill_information
    //Location Name
    size_t i = location_data.GetLocationName().find(s_delimiter, 0);
    if (i != std::string::npos) 
    {
        m_image.AddImageText(s_black,m_location_1,12,location_data.GetLocationName().substr(0,i),false);
        i++; 
        m_image.AddImageText(s_black,m_location_2,12,location_data.GetLocationName().substr(i),false);
    } else 
    {
        m_image.AddImageText(s_black,m_location_1,12,location_data.GetLocationName().substr(0),false);
    }

    std::string health = std::to_string(player.stats().current_health()) + "/" + std::to_string(player.stats().max_health());
    std::string gold = std::to_string(player.gold());
    m_image.AddImageText(s_black,m_current_health,12,health,false);
    m_image.AddImageText(s_black,m_gold,12,gold,false);
    
#pragma endregion

#pragma region fill_map
    std::ifstream map_in{location_data.GetLocationImage(),std::ios::binary};
    GD::Image map{map_in,GD::Png_tag{}};
    if (map.good()) 
    {
        GD::Size map_size{};
        map.GetSize(map_size);
        m_image.Copy(map, m_map_start,GD::Point{0,0},map_size);
    }    
#pragma endregion
    
#pragma region interactions
    for (auto const &interaction : location_data) 
    {   
        int index = 0;
        if (interaction.GetDatabaseId() != -1) 
        {
            index = interaction.GetMainType()->CalculateImageIndex(location_db.interactions(interaction.GetDatabaseId()));
        }
        std::ifstream interaction_in{interaction.GetIconMapPath(index),std::ios::binary};
        GD::Image interaction_img{interaction_in,GD::Png_tag{}};
        m_image.Copy(interaction_img,GD::Point{map_x_offset + interaction.GetPosX(),map_y_offset + interaction.GetPosY()},GD::Point{0,0},s_map_icon_size);
    }
#pragma endregion

    return true;
}

bool Renderer::PostMapRenderer::FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db)
{
    BaseMapRenderer::FillContents(player, location_data,location_db);
    const DCLData::DCLInteraction *interaction_data = location_data.GetInteraction(m_selected);
    m_image.AddImageText(s_black,s_post_selected,10,interaction_data->GetInteractionName(),true);

    const auto &posX = interaction_data->GetPosX();
    const auto &posY = interaction_data->GetPosY();
    GD::Point selectedPos{posX + map_x_offset,posY + map_y_offset};
    std::ifstream in_selected{s_selected};
    GD::Image selected_img{in_selected};
    m_image.Copy(selected_img,selectedPos,{0,0},s_map_icon_size);

    const auto &post_info_db = location_db.interactions(interaction_data->GetDatabaseId()).post_info();
    const DCLInteractions::DCLPostInteraction *post_info = interaction_data->TryGetPost();
    auto const &item_info = DCLData::DCLItems::getInstance();
    int i = 0;
    
    for(auto const &resource : post_info->GetResources()) 
    {
        const std::string *item_name = item_info.GetItemName(resource.item_id());

        m_image.AddImageText(s_white,{s_starting_resource_list.X(),s_starting_resource_list.Y() + (s_listy * i)},10,*item_name,false);
        if (m_notify_items)
            for (auto const& item: m_items)
            {
                if (resource.item_id() == item.item_id()) 
                {
                    m_image.AddImageText(s_white,{s_starting_resource_list.X() + 80,s_starting_resource_list.Y() + (s_listy * i)},10,"+ "+std::to_string(item.quantity()),false);
                }
            }
        
        i++;
    }
    //CAPACITY
    float capacity = post_info->GetCurrentStat(PBUpgradeType::CAPACITY,post_info_db.capacity_upgrade());
    float stored_percent = post_info_db.resource_stored() / capacity;
    m_image.FilledRectangle(s_start_meter,GD::Point{s_end_meter.X(),s_start_meter.Y() - static_cast<int>(66*stored_percent)},s_white.Int());
    std::string text = std::to_string(static_cast<int>(capacity)) + "u";
    m_image.AddImageText(s_white,s_capacity,9,text,false);
    
    //REGENERATION
    std::stringstream stream_regen;
    stream_regen << std::fixed << std::setprecision(2) << post_info->GetCurrentStat(PBUpgradeType::GEN_SECOND,post_info_db.gen_second_upgrade());
    std::string text2 = stream_regen.str() + "/s";
    m_image.AddImageText(s_white,s_regeneration,9,text2,false);
    
    //FORTUNE
    std::stringstream stream_fortune;
    stream_fortune << std::fixed << std::setprecision(2) << post_info->GetCurrentStat(PBUpgradeType::FORTUNE,post_info_db.fortune_upgrade());
    std::string text3 = stream_fortune.str() + "%";
    m_image.AddImageText(s_white,s_fortune,9,text3,false);
    return true;
}

bool Renderer::ZoneAccessRenderer::FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db)
{
    BaseMapRenderer::FillContents(player, location_data,location_db);
    const DCLData::DCLInteraction *interaction_data =location_data.GetInteraction(m_selected);
    size_t i = interaction_data->GetInteractionName().find(s_delimiter, 0);
    if (i != std::string::npos) 
    {
        m_image.AddImageText(s_black,s_zone_access_name1,12,interaction_data->GetInteractionName().substr(0,i),false);
        i++; 
        m_image.AddImageText(s_black,s_zone_access_name2,12,interaction_data->GetInteractionName().substr(i),false);
    } else 
    {
        m_image.AddImageText(s_black,s_zone_access_name1,12,interaction_data->GetInteractionName().substr(0),false);
    }

    auto const &posX = interaction_data->GetPosX();
    auto const &posY = interaction_data->GetPosY();
    GD::Point selectedPos{posX + map_x_offset,posY + map_y_offset};
    std::ifstream in_selected{s_selected};
    GD::Image selected_img{in_selected};
    m_image.Copy(selected_img,selectedPos,{0,0},s_map_icon_size);
    if (!m_unlocked) 
    {
        auto const &zone_access_info_db = location_db.interactions(interaction_data->GetDatabaseId()).zone_access_info();
        auto const &zone_access_info_data = interaction_data->TryGetZoneAccess();
        auto const &item_info = DCLData::DCLItems::getInstance();
        int j = 0;
        for(auto const &resource : zone_access_info_data->GetRequirements(zone_access_info_db.unlock_level())) 
        {
            std::string item_name = (*item_info.GetItemName(resource.item_id())) +" - "+ std::to_string(resource.quantity());
            m_image.AddImageText(s_white,{s_required_list.X(), s_required_list.Y() + static_cast<int>(s_increment_y * j)},10,item_name,false);
            j++;
        }
    }
    
    return true;
};

bool Renderer::DialogRender::FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db)
{
    BaseMapRenderer::FillContents(player, location_data,location_db);
    const DCLData::DCLInteraction *interaction_data =location_data.GetInteraction(m_selected);
    size_t i = interaction_data->GetInteractionName().find(s_delimiter, 0);
    if (i != std::string::npos) 
    {
        m_image.AddImageText(s_black,s_dialog_name1,12,interaction_data->GetInteractionName().substr(0,i),false);
        i++; 
        m_image.AddImageText(s_black,s_dialog_name2,12,interaction_data->GetInteractionName().substr(i),false);
    } else 
    {
        m_image.AddImageText(s_black,s_dialog_name1,12,interaction_data->GetInteractionName().substr(0),false);
    }

    auto const &posX = interaction_data->GetPosX();
    auto const &posY = interaction_data->GetPosY();
    GD::Point selectedPos{posX + map_x_offset,posY + map_y_offset};
    std::ifstream in_selected{s_selected};
    GD::Image selected_img{in_selected};
    m_image.Copy(selected_img,selectedPos,{0,0},s_map_icon_size);
    auto const *dialog_info = interaction_data->TryGetDialog();
    std::ifstream if_npc_image{dialog_info->GetFirstDialogImage()};
    GD::Image npc_image{if_npc_image};
    m_image.Copy(npc_image,s_dialog_image,{0,0},GD::Size{64,64});
    
    return true;
};



std::unique_ptr<Renderer::BaseInventoryRenderer> Renderer::InventoryRendererFactory::CreateRenderer(int type) 
{

    switch (type)
    {
    case PBItemType::RESOURCES:
        return std::make_unique<Renderer::ResourceInventoryRenderer>();
    case PBItemType::ATTACKS:
        return std::make_unique<Renderer::ResourceInventoryRenderer>();
    case PBItemType::WEAPONS:
        return std::make_unique<Renderer::ResourceInventoryRenderer>();
    
    default:
        return std::make_unique<Renderer::ResourceInventoryRenderer>();
    }
}
