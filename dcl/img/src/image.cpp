#include <img/image.hpp>

using namespace Rendering;

DCLImage::DCLImage(std::string &input) 
{
    std::ifstream img(input, std::ios::binary);
    image.CreateFrom(img);
    if (image.good())
    {
        std::string end_path = "test/img_tests/test1/out_roomba.png";
        
        std::ofstream out(end_path, std::ios_base::out | std::ios_base::binary );
        image.Png(out);
    }
}