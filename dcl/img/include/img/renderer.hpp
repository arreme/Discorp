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
        const static int16_t initial_x = 80;
        const static int16_t initial_y = 18;
        const static int16_t increment = 88; 
    public:
        InventoryRender(const std::string &inventory);
        void AddItemToInventory(const std::string &item_image);
    };

    

    extern void FreePointer(char * data);
}
