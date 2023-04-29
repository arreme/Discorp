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
        bool m_good;
        GD::Image m_image;
    public:
        std::unique_ptr<char, void(*)(char*)> RenderImage(int *size);
        ImageRenderer(const std::string &base_image);
    };
    extern void FreePointer(char * data);


}
