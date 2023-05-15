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
        int index = interaction.GetMainType()->CalculateImageIndex(location_db.interactions(interaction.GetDatabaseId()));
        std::ifstream interaction_in{interaction.GetIconMapPath(index),std::ios::binary};
        GD::Image interaction_img{interaction_in,GD::Png_tag{}};
        m_image.Copy(interaction_img,GD::Point{map_x_offset + interaction.GetPosX(),map_y_offset + interaction.GetPosY()},GD::Point{0,0},s_map_icon_size);
    }
#pragma endregion

    return true;
}