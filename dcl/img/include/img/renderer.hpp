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

        bool FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db) override
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
                std::cout << resource.DebugString() << std::endl;
                const std::string *item_name = item_info.GetItemName(resource.item_id());
                m_image.AddImageText(s_white,{s_starting_resource_list.X(),s_starting_resource_list.Y() + (s_listy * i)},10,*item_name,false);
                i++;
            }
            //CAPACITY
            int capacity = post_info->GetCurrentStat(PBUpgradeType::CAPACITY,post_info_db.capacity_upgrade());
            int stored_percent = post_info_db.resource_stored() / capacity;
            m_image.FilledRectangle(s_start_meter,GD::Point{s_end_meter.X(),s_start_meter.Y() + static_cast<int>(66*stored_percent)},s_white.Int());
            std::string text = std::to_string(capacity) + "u";
            m_image.AddImageText(s_white,s_capacity,9,text,false);
            
            //REGENERATION
            std::stringstream stream_regen;
            stream_regen << std::fixed << std::setprecision(2) << post_info->GetCurrentStat(PBUpgradeType::GEN_SECOND,post_info_db.gen_second_upgrade());;
            std::string text2 = stream_regen.str() + "/s";
            m_image.AddImageText(s_white,s_regeneration,9,text2,false);
            
            //FORTUNE
            std::stringstream stream_fortune;
            stream_fortune << std::fixed << std::setprecision(2) << post_info->GetCurrentStat(PBUpgradeType::FORTUNE,post_info_db.fortune_upgrade());;
            std::string text3 = stream_fortune.str() + "%";
            m_image.AddImageText(s_white,s_fortune,9,text3,false);
            return true;
        };
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

        virtual bool FillContents(const PBPlayer &player, const DCLData::DCLLocation &location_data, const PBLocation &location_db) override
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

            const auto &posX = interaction_data->GetPosX();
            const auto &posY = interaction_data->GetPosY();
            GD::Point selectedPos{posX + map_x_offset,posY + map_y_offset};
            std::ifstream in_selected{s_selected};
            GD::Image selected_img{in_selected};
            m_image.Copy(selected_img,selectedPos,{0,0},s_map_icon_size);

            const auto &zone_access_info_db = location_db.interactions(interaction_data->GetDatabaseId()).zone_access_info();
            const auto &zone_access_info_data = interaction_data->TryGetZoneAccess();
            auto const &item_info = DCLData::DCLItems::getInstance();
            int j = 0;
            for(auto const &resource : zone_access_info_data->GetRequirements(zone_access_info_db.unlock_level())) 
            {
                std::string item_name = (*item_info.GetItemName(resource.item_id())) +" - "+ std::to_string(resource.quantity());
                m_image.AddImageText(s_white,{s_required_list.X(), s_required_list.Y() + static_cast<int>(s_increment_y * j)},10,item_name,false);
                j++;
            }
            return true;
        };
    };
}
