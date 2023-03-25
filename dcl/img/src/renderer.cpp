#include <img/renderer.hpp>


Renderer::LocationRender::LocationRender(const std::string &locationPath) 
{
    std::ifstream in{locationPath, std::ios::binary};
    image = GD::Image{in};
    std::cout << image.good() << std::endl;
}

void Renderer::LocationRender::AddInteraction(int posX, int posY, const std::string &interaction_image)
{
    std::ifstream in{interaction_image, std::ios::binary};
    GD::Image interaction{in};
    std::cout << interaction.good() << std::endl;
    GD::Size s;
    interaction.GetSize(s);
    std::cout << s.H() << std::endl;
    std::cout << s.W() << std::endl;
    GD::Point p{posX,posY};

    image.Copy(interaction,p,GD::Point{0,0},s);
}

std::unique_ptr<char, void(*)(char*)> Renderer::LocationRender::RenderLocation(int *size) 
{   
    return std::unique_ptr<char, void(*)(char*)>{(char *) image.Png(size), Renderer::FreePointer};
}