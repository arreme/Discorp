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
        const int m_selected = -1;
        std::vector<PBItemData> m_items;
        bool m_notify_items = false;;
    public:
        PostMapRenderer(int selected) 
        : BaseMapRenderer(s_post_map), m_selected(selected)
        {};

        void NotifyNewItems(std::vector<PBItemData> &items) 
        {
            m_items = items;
            m_notify_items = true;
        }

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


    class BaseInventoryRenderer : public ImageRenderer 
    {
    protected:

        inline static const std::string s_uis[PBItemType_ARRAYSIZE] = 
            {
                "resources/assets/UI/inventory_resources.png",
                "resources/assets/UI/inventory_attacks.png",
                "resources/assets/UI/inventory_weapons.png",
                "resources/assets/UI/inventory_armor.png",
                "resources/assets/UI/inventory_quest_items.png",
                "resources/assets/UI/inventory_buids.png"
            };
        PBItemType m_selected;

        inline static const GD::Point s_initial_name{175,50};
        inline static const GD::Point s_initial_quantity{174,31};
        inline static const GD::Point s_initial_image{140,21};
        inline static const GD::Point s_initial_gold{220,31};

        inline static const GD::Size s_inv_image_size{32,32};
        inline static const int x_displace = 179;
        inline static const int y_displace = 43;

    public:
        BaseInventoryRenderer(PBItemType selected) 
        : ImageRenderer(s_uis[selected]), m_selected(selected)
        {}

        virtual ~BaseInventoryRenderer(){ }
        
        virtual void FillContent(std::vector<PBItemData> &items_db, int page)
        {
            auto const &item_data = DCLData::DCLItems::getInstance();
            int index = page * 8;
            for(int x = index; x < items_db.size() && x < 8; x++) 
            {
                int i = x % 4;
                int j = x / 4;
                PBItemEnum item_id = items_db.at(x).item_id();
                const std::string *item_name = item_data.GetItemName(item_id);
                
                m_image.AddImageText(s_white,{s_initial_name.X() + (x_displace * j),s_initial_name.Y() + (y_displace * i)},12,*item_name,true);
                m_image.AddImageText(s_white,{s_initial_quantity.X() + (x_displace * j),s_initial_quantity.Y() + (y_displace * i)},10,std::to_string(items_db.at(x).quantity()),true);
                m_image.AddImageText(s_black,{s_initial_gold.X() + (x_displace * j),s_initial_gold.Y() + (y_displace * i)},10,std::to_string(item_data.GetItemGold(item_id)),true);
                
                const std::string *item_path = item_data.GetItemPath(item_id);

                std::ifstream img_path{*item_path};
                GD::Image item_image{img_path};
                if (item_image.good())
                    m_image.Copy(item_image,{s_initial_image.X() + (x_displace * j),s_initial_image.Y() + (y_displace * i)},{0,0},s_inv_image_size);
            }
        };
    };

    class ResourceInventoryRenderer : public BaseInventoryRenderer 
    {
    public:

        ResourceInventoryRenderer() 
        : BaseInventoryRenderer(PBItemType::RESOURCES)
        {}

        void FillContent(std::vector<PBItemData> &items_db, int page) override
        {
            BaseInventoryRenderer::FillContent(items_db,page);
        }
        
    };

    class InventoryRendererFactory
    {
    public:
        static std::unique_ptr<BaseInventoryRenderer> CreateRenderer(int type);
    };
}
