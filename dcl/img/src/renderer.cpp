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

bool Renderer::BaseMapRenderer::FillContents(const PBPlayer &player, const PBLocation &location_data, const PBLocation &location_db) 
{
    if (!m_image.good()) return false;
#pragma region fill_location
    size_t i = location_data.name().find(s_delimiter, 0);
    if (i != std::string::npos) 
    {
        m_image.AddImageText(s_black,m_location_1,12,location_data.name().substr(0,i),false);
        i++; 
        m_image.AddImageText(s_black,m_location_2,12,location_data.name().substr(i),false);
    } else 
    {
        m_image.AddImageText(s_black,m_location_1,12,location_data.name().substr(0),false);
    }
    
#pragma endregion

#pragma region fill_map
    std::ifstream map_in{location_data.image(),std::ios::binary};
    GD::Image map{map_in,GD::Png_tag{}};
    if (map.good()) 
    {
        GD::Size map_size{};
        map.GetSize(map_size);
        m_image.Copy(map, m_map_start,GD::Point{0,0},map_size);
    }
    
#pragma endregion

#pragma region fill_stats
    std::string health = std::to_string(player.stats().current_health()) + "/" + std::to_string(player.stats().max_health());
    std::string gold = std::to_string(player.gold());
    m_image.AddImageText(s_black,m_current_health,12,health,false);
    m_image.AddImageText(s_black,m_gold,12,gold,false);
#pragma endregion
    
#pragma region interactions
    for (auto const &interaction : location_data.interactions()) 
    {
        auto &interaction_db = location_db.interactions(interaction.database_id());
        int index = 0;
        switch(interaction.types(0)) 
        {
            case PBInteractionType::POST:
            {
                auto post_db = interaction_db.post_info();
                index = (post_db.capacity_upgrade() + post_db.gen_second_upgrade() + post_db.fortune_upgrade()) / 3;
                index = std::min(index, interaction.map_icon_paths().size() - 1);
                break;
            }
            case PBInteractionType::ZONE_ACCESS:
            {
                index = interaction_db.zone_access_info().unlock_level();
                break;
            }
            case PBInteractionType::DIALOG:
            {
                index = 0;
                break;
            }
            default:
                index = 0;
                break;
        }
        std::ifstream interaction_in{interaction.map_icon_paths(index),std::ios::binary};
        GD::Image interaction_img{interaction_in,GD::Png_tag{}};
        m_image.Copy(interaction_img,GD::Point{map_x_offset + interaction.pos_x(),map_y_offset + interaction.pos_y()},GD::Point{0,0},s_map_icon_size);

    }
#pragma endregion

    return true;
}