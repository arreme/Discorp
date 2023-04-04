#include <img/renderer.hpp>

Renderer::ImageRenderer::ImageRenderer(const std::string &locationPath) 
{
    std::ifstream in{locationPath, std::ios::binary};
    image = GD::Image{in};
}

Renderer::LocationRender::LocationRender(const std::string &locationPath)
: ImageRenderer(locationPath)
{}

Renderer::InventoryRender::InventoryRender(const std::string &inventory)
: ImageRenderer(inventory)
{}


void Renderer::LocationRender::AddInteraction(int posX, int posY, const std::string &interaction_image)
{
    std::ifstream in{interaction_image, std::ios::binary};
    GD::Image interaction{in};
    GD::Size s;
    interaction.GetSize(s);
    GD::Point p{posX,posY};

    image.Copy(interaction,p,GD::Point{0,0},s);
}

void Renderer::InventoryRender::AddItemToInventory(const std::string &item_image) 
{
    if (current >= 8) return;
    auto x = initial_x + current % 4;
    auto y = initial_y + current / 4;

    current++;

    std::ifstream in{item_image, std::ios::binary};
    GD::Image interaction{in};
    GD::Size s;
    interaction.GetSize(s);
    GD::Point p{x,y};

    image.Copy(interaction,p,GD::Point{0,0},s);
}

std::unique_ptr<char, void(*)(char*)> Renderer::ImageRenderer::RenderImage(int *size) 
{   
    return std::unique_ptr<char, void(*)(char*)>{(char *) image.Png(size), Renderer::FreePointer};
}

void Renderer::FreePointer(char * data) 
{
    gdFree((void *)data);
}