#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>

namespace Renderer
{
    class LocationRender 
    {
        int16_t interaction_number = 0;
        GD::Image image;
    public:
        LocationRender(const std::string &locationPath);

        void AddInteraction(int posX, int posY, const std::string &interaction_image);

        std::unique_ptr<char, void(*)(char*)> RenderLocation(int *size);
    };

    

    void FreePointer(char * data) 
    {
        std::cout << "I'm freeing your pointer" << std::endl;
        gdFree((void *)data);
    }
}
