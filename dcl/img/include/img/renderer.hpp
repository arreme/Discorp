#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>

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
}
