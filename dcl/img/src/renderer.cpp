#include <img/renderer.hpp>


Renderer::LocationRender::LocationRender(const std::string &locationPath) 
{
    std::ifstream in{locationPath, std::ios::binary};
    image = GD::Image{in};
}

void Renderer::LocationRender::AddInteraction(int posX, int posY, const std::string &interaction_image)
{
    std::ifstream in{interaction_image, std::ios::binary};
    GD::Image interaction{in};
    GD::Size s;
    interaction.GetSize(s);
    GD::Point p{posX,posY};

    image.Copy(interaction,p,GD::Point{0,0},s);
}

std::unique_ptr<char, void(*)(char*)> Renderer::ImageRenderer::RenderImage(int *size) 
{   
    return std::unique_ptr<char, void(*)(char*)>{(char *) image.Png(size), Renderer::FreePointer};
}