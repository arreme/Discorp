#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>
#include <math.h>
#include <core/pb/player.pb.h>
#include <core/pb/map.pb.h>
#include <sstream>
#include <iomanip>
#include <core/dcl_items.hpp>
#include <core/dcl_map.hpp>

namespace Renderer
{
    class ImageRenderer 
    {
    protected:
        inline static const GD::TrueColor s_white{0xFF,0xFF,0xFF};
        inline static const GD::TrueColor s_black{0x00,0x00,0x00};
        inline static const GD::TrueColor s_light_cyan{0x00,0xFF,0xFF};
        inline static const std::string s_delimiter = "|";
        inline static const std::string s_selected = "resources/assets/MapIcons/selected.png";

        GD::Image m_image;
    public:
        std::unique_ptr<char, void(*)(char*)> RenderImage(int *size);
        ImageRenderer(const std::string &base_image);
    };
    extern void FreePointer(char * data);

    class BaseMapRenderer : public ImageRenderer 
    {
    protected:
        inline static constexpr int map_x_offset = 170;
        inline static constexpr int map_y_offset = 14;
        inline static const std::string s_base_map = "resources/assets/UI/nothing_selected.png";
        inline static const GD::Size s_map_icon_size{32,32};
        inline static const GD::Point m_map_start{170,14};
        inline static const GD::Point m_location_1{17,41};
        inline static const GD::Point m_location_2{17,54};
        inline static const GD::Point m_current_health{17,314};
        inline static const GD::Point m_gold{17,330};
    public:
        BaseMapRenderer(std::string path = s_base_map) 
        : ImageRenderer(path)
        {};

        virtual bool FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db);
    };

    class PostMapRenderer : public BaseMapRenderer 
    {
    private:
        inline static const GD::Point s_post_selected{17,83};
        inline static const GD::Point s_starting_resource_list{49,117};
        inline static const int s_listy = 12;
        inline static const GD::Point s_start_meter{19,184};
        inline static const GD::Point s_end_meter{32,119};
        inline static const GD::Point s_capacity{49,233};
        inline static const GD::Point s_regeneration{49,264};
        inline static const GD::Point s_fortune{49,295};
        inline static const std::string s_post_map = "resources/assets/UI/post_selected.png";
        const int m_selected;
    public:
        PostMapRenderer(int selected) 
        : BaseMapRenderer(s_post_map), m_selected(selected)
        {};

        bool FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db) override;
    };

    class ZoneAccessRenderer : public BaseMapRenderer 
    {
        inline static const GD::Point s_zone_access_name1{17,83};
        inline static const GD::Point s_zone_access_name2{17,96};
        inline static const GD::Point s_required_list{22,217};
        inline static const int s_increment_y = 12;
        inline static const std::string s_unlocked = "resources/assets/UI/zone_access_unlocked_selected.png";
        inline static const std::string s_locked = "resources/assets/UI/zone_access_locked_selected.png";
        const int m_selected;
        const int m_unlocked;
    public:
        ZoneAccessRenderer(int selected, bool unlocked) 
        : BaseMapRenderer(unlocked ? s_unlocked : s_locked), m_selected(selected), m_unlocked(unlocked)
        {};

        bool FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db) override;
    };
}
