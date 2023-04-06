#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <img/gdpp.hpp>

TEST_CASE("Image first test","[image_1]") 
{
    std::string path = "test/img_tests/test1/roombe.png";
    std::ifstream in{path, std::ios::binary};
    GD::Image image{in};
    REQUIRE_FALSE(image);

    std::string path2 = "test/img_tests/test1/roomba.png";
    std::ifstream in2{path2, std::ios::binary};
    GD::Image image2{in2};

    std::ifstream in3{path2, std::ios::binary};
    GD::Image image3{in3};
    REQUIRE(image2);
    REQUIRE(image3);
    REQUIRE(image2 == image3);
    GD::Point point{10,10};
    GD::Size size{10,10};
    GD::TrueColor color{0xFF,0,0};
    image2.FilledRectangle(point,size,color.Int());
    REQUIRE_FALSE(image2 == image3);
    GD::TrueColor color2{0xFF,0xFF,0};
    GD::Point point2{40,30};
    std::string text{"Hey World!"};
    image2.AddImageText(color2,point2,20,text,true);

    std::string path3 = "test/img_tests/test2.png";
    std::ifstream in4{path3, std::ios::binary};
    GD::Image image4{in4};
    REQUIRE(image4.good());
    GD::Size s;
    image4.GetSize(s);
    image2.Copy(image4,GD::Point{0,0},GD::Point{0,0},s);
    std::ofstream out("test/img_tests/test1/image.png", std::ios_base::out | std::ios_base::binary );
    image2.Png(out);
    
}


TEST_CASE("Letters on image","[img_letters]") 
{
    std::string path = "resources/assets/UI/resources.png";
    std::ifstream in{path, std::ios::binary};
    GD::Image image{in};
    GD::Point p{58,92};
    GD::Point p2{58,34};
    GD::TrueColor color{0xFF,0xFF,0xFF};
    std::string item_name = "W. Helmet";
    std::string item_quantity = "103";
    image.AddImageText(color,p,10,item_name,false);
    image.AddImageText(color,p2,10,item_quantity,false);
    std::ofstream out("test/img_tests/test2/image.png", std::ios_base::out | std::ios_base::binary );
    image.Png(out);
}