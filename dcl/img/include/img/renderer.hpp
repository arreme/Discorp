#pragma once
#include <img/gdpp.hpp>
#include <fstream>
#include <memory>

namespace Renderer
{
    class LocationRender 
    {
    public:
        LocationRender(std::string locationPath);

    };

    void RenderLocation(std::string &data);

    void FreePointer(char * data) 
    {
        std::cout << "I'm freeing your pointer" << std::endl;
        gdFree((void *)data);
    }
}
