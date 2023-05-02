#include <img/renderer.hpp>


Renderer::ImageRenderer::ImageRenderer(const std::string &base_image) 
{
    std::ifstream in{base_image, std::ios::binary};
    m_image = GD::Image{in,GD::Png_tag{}};
    m_good = m_image.good();
    
}

std::unique_ptr<char, void(*)(char*)> Renderer::ImageRenderer::RenderImage(int *size) 
{
    return std::unique_ptr<char, void(*)(char*)>{(char *) m_image.Png(size), Renderer::FreePointer};
}

void Renderer::FreePointer(char * data) 
{
    gdFree((void *)data);
}