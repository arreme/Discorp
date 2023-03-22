#pragma once

#include <img/gdpp/gdpp.h>
#include <fstream>

namespace Rendering 
{
    class DCLImage 
    {
        GD::Image image;
    public:
        DCLImage(std::string &input);
        bool operator==(GD::Image const& rhs) const 
        {
            //return tied() == rhs.tied();
            return false; 
        }
    };
}
