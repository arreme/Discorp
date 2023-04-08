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
{

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

void Renderer::InventoryRender::AddItemToInventory(const std::string &item_image, std::string item_name, std::string item_quantity) 
{
    if (current >= 8) return;
    auto imgX = initial_x + (increment_x * (current % 2));
    auto imgY = initial_y + (increment_y * (current / 2));

    auto numX= 77 + (increment_x * (current % 2));
    auto numY= 75 + (increment_y * (current / 2));

    auto nameX = 77 + (increment_x * (current % 2));
    auto nameY = 109 + (increment_y * (current / 2));
    current++;
    std::ifstream in{item_image, std::ios::binary};
    GD::Image interaction{in};
    GD::Size s;
    interaction.GetSize(s);
    GD::Point imgP{imgX,imgY};
    GD::Point nameP{nameX,nameY};
    GD::Point numP{numX,numY};
    GD::TrueColor color2{0xFF,0xFF,0xFF};
    image.Copy(interaction,imgP,GD::Point{0,0},s);
    image.AddImageText(color2, nameP, 12,item_name, false);
    image.AddImageText(color2, numP, 12,item_quantity,false);
}

std::unique_ptr<char, void(*)(char*)> Renderer::ImageRenderer::RenderImage(int *size) 
{
    return std::unique_ptr<char, void(*)(char*)>{(char *) image.Png(size), Renderer::FreePointer};
}

void Renderer::FreePointer(char * data) 
{
    gdFree((void *)data);
}