#include <catch2/catch_test_macros.hpp>
#include <img/image.hpp>

TEST_CASE("Image first test","[image_1]") 
{
    std::string path = "test/img_tests/test1/roomba.png";
    Rendering::DCLImage image{path};
}