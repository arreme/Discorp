#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>
#include <math.h>
#include <core/pb/player.pb.h>
#include <core/pb/map.pb.h>
#include <sstream>
#include <iomanip>

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

        virtual bool FillContents(const PBPlayer &player, const PBLocation &location_data, const PBLocation &location_db);
    };

    class PostMapRenderer : public BaseMapRenderer 
    {
    private:
        inline static const GD::Point s_post_selected{17,83};
        inline static const GD::Point s_starting_resource_list{49,117};
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

        virtual bool FillContents(const PBPlayer &player, const PBLocation &location_data, const PBLocation &location_db) override
        {
            BaseMapRenderer::FillContents(player, location_data,location_db);
            const auto &post_info_db = location_db.interactions(location_data.interactions(m_selected).database_id()).post_info();
            const auto &post_info = location_data.interactions(m_selected).post_info();
            int capacity = post_info.upgrades(PBUpgradeType::CAPACITY).info(post_info_db.capacity_upgrade()).current_stat();
            std::string text = std::to_string(capacity) + "- LVL" + std::to_string(post_info_db.capacity_upgrade());
            m_image.AddImageText(s_white,s_capacity,9,text,false);
            std::stringstream stream_regen;
            stream_regen << std::fixed << std::setprecision(2) << post_info.upgrades(PBUpgradeType::GEN_SECOND).info(post_info_db.gen_second_upgrade()).current_stat();
            std::string text2 = stream_regen.str() + "- LVL" + std::to_string(post_info_db.gen_second_upgrade());
            m_image.AddImageText(s_white,s_regeneration,9,text2,false);
            std::stringstream stream_fortune;
            stream_fortune << std::fixed << std::setprecision(2) << post_info.upgrades(PBUpgradeType::FORTUNE).info(post_info_db.fortune_upgrade()).current_stat();
            std::string text3 = stream_fortune.str() + "- LVL" + std::to_string(post_info_db.fortune_upgrade());
            m_image.AddImageText(s_white,s_fortune,9,text3,false);
            return true;
        };
    };
}
