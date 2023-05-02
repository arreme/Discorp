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
        bool m_good;
        GD::Image m_image;
    public:
        std::unique_ptr<char, void(*)(char*)> RenderImage(int *size);
        ImageRenderer(const std::string &base_image);
    };
    extern void FreePointer(char * data);

    class BaseMapRenderer : public ImageRenderer 
    {
    private:
        inline static const std::string m_base_map_path = "resources/assets/UI/nothing_selected.png";
        const GD::Point m_map_start{170,14};
        const GD::Point m_location_1{17,41};
        const GD::Point m_location_2{17,54};
        const GD::Point m_current_health{17,314};
        const GD::Point m_gold{17,330};
    public:
        BaseMapRenderer() 
        : ImageRenderer(m_base_map_path)
        {};

        bool FillContents(const PBPlayer &player, const PBLocation &location_data) 
        {
            if (!m_good) return false;
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
            return true;
        };
    };
}
