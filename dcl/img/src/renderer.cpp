#include <img/renderer.hpp>


void Renderer::RenderLocation(std::string &data) 
{
    std::string path = "test/img_tests/test1/roombe.png";
    std::ifstream in{path, std::ios::binary};
    GD::Image image{in};
    int size = -1;
    
    std::unique_ptr<char, void(*)(char*)> ptr{(char *) image.Png(&size), Renderer::FreePointer};
}