#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>
#include <math.h>

namespace Renderer
{
    class ImageRenderer 
    {
    protected:
        GD::Image image;
    public:
        std::unique_ptr<char, void(*)(char*)> RenderImage(int *size);
        ImageRenderer(const std::string &locationPath);
    };

    class LocationRender : public ImageRenderer
    {
    private:
        int16_t interaction_number = 0;
    public:
        LocationRender(const std::string &locationPath);

        void AddInteraction(int posX, int posY, const std::string &interaction_image);
    };

    class InventoryRender : public ImageRenderer 
    {
    private:
        int16_t current = 0;
        const static int16_t initial_x = 20;
        const static int16_t initial_y = 58;
        const static int16_t increment_x = 198;
        const static int16_t increment_y = 64; 
    public:
        InventoryRender(const std::string &inventory);
        void AddItemToInventory(const std::string &item_image, std::string item_name, std::string item_quantity);
    };

    extern void FreePointer(char * data);

    class PlayerRenderer : public ImageRenderer 
    {
    private:
        inline const static std::string image_path = "resources/assets/UI/PlayerUI.png";
    public:
        PlayerRenderer(uint32_t max_health, uint32_t current_health, int32_t strength, int32_t defense, int32_t precision, int32_t speed, int32_t luck, uint32_t forage_lvl, uint32_t forage_xp, float forage_xp_percent, uint32_t mining_lvl, uint32_t mining_xp, float mining_xp_percent, uint32_t combat_lvl, uint32_t combat_xp, float combat_xp_percent, uint32_t athletics_lvl, uint32_t athletics_xp, float athletics_xp_percent);
    };


    class PostRenderer : public ImageRenderer 
    {
    private:
        inline const static std::string image_path = "resources/assets/UI/PostUI.png";
    public:
        PostRenderer(int32_t capacity, int capacity_value, float filled_percent, int32_t gen_second, float gen_second_value, int32_t fortune, float fortune_value, std::string post_image);
    };
}
