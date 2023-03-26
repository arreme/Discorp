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
    };

    class LocationRender : public ImageRenderer
    {
    private:
        int16_t interaction_number = 0;
    public:
        LocationRender(const std::string &locationPath);

        void AddInteraction(int posX, int posY, const std::string &interaction_image);
    };

    

    void FreePointer(char * data) 
    {
        std::cout << "I'm freeing your pointer" << std::endl;
        gdFree((void *)data);
    }
}
