#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>
#include <math.h>
#include <core/pb/player.pb.h>
#include <core/pb/map.pb.h>

namespace Renderer
{
    class ImageRenderer 
    {
    protected:
        inline static const GD::TrueColor s_white{0xFF,0xFF,0xFF};
        inline static const GD::TrueColor s_black{0x00,0x00,0x00};
        inline static const GD::TrueColor s_light_cyan{0x00,0xFF,0xFF};
        inline static const std::string s_delimiter = "|";

        GD::Image m_image;
    public:
        std::unique_ptr<char, void(*)(char*)> RenderImage(int *size);
        ImageRenderer(const std::string &base_image);
    };
    extern void FreePointer(char * data);

    class BaseMapRenderer : public ImageRenderer 
    {
    private:
        inline static constexpr int map_x_offset = 170;
        inline static constexpr int map_y_offset = 14;
        inline static const std::string s_base_map_path = "resources/assets/UI/nothing_selected.png";
        inline static const GD::Size s_map_icon_size{32,32};
        inline static const GD::Point m_map_start{170,14};
        inline static const GD::Point m_location_1{17,41};
        inline static const GD::Point m_location_2{17,54};
        inline static const GD::Point m_current_health{17,314};
        inline static const GD::Point m_gold{17,330};
    public:
        BaseMapRenderer() 
        : ImageRenderer(s_base_map_path)
        {};

        bool FillContents(const PBPlayer &player, const PBLocation &location_data, const PBLocation &location_db) 
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
                switch(interaction.types(0)) 
                {
                    case PBInteractionType::POST:
                    {
                        auto post_db = location_db.interactions(interaction.database_id()).post_info();
                        int index = (post_db.capacity_upgrade() + post_db.gen_second_upgrade() + post_db.fortune_upgrade()) / 3;
                        index = std::min(index, interaction.map_icon_paths().size() - 1);
                        std::ifstream post_in{interaction.map_icon_paths(index),std::ios::binary};
                        GD::Image post{post_in,GD::Png_tag{}};
                        m_image.Copy(post,GD::Point{map_x_offset + interaction.pos_x(),map_y_offset + interaction.pos_y()},GD::Point{0,0},s_map_icon_size);
                        break;
                    }
                    case PBInteractionType::ZONE_ACCESS:
                    {
                        auto zone_access_db = location_db.interactions(interaction.database_id()).zone_access_info();
                        std::ifstream zone_access_in{interaction.map_icon_paths(zone_access_db.unlock_level()),std::ios::binary};
                        GD::Image zone_access{zone_access_in,GD::Png_tag{}};
                        m_image.Copy(zone_access,GD::Point{map_x_offset + interaction.pos_x(),map_y_offset + interaction.pos_y()},GD::Point{0,0},s_map_icon_size);
                        break;
                    }
                    case PBInteractionType::DIALOG:
                    {
                        std::ifstream dialog_in{location_data.image(),std::ios::binary};
                        GD::Image dialog{dialog_in,GD::Png_tag{}};
                        break;
                    }
                } 
            }
#pragma endregion

            return true;
        };
    };
}
