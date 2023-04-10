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

Renderer::PlayerRenderer::PlayerRenderer(uint32_t max_health, uint32_t current_health, int32_t strength, int32_t defense, int32_t precision, int32_t speed, int32_t luck, uint32_t forage_lvl, uint32_t forage_xp, float forage_xp_percent, uint32_t mining_lvl, uint32_t mining_xp, float mining_xp_percent, uint32_t combat_lvl, uint32_t combat_xp, float combat_xp_percent, uint32_t athletics_lvl, uint32_t athletics_xp, float athletics_xp_percent) 
: ImageRenderer(image_path)
{
    std::string text;
    GD::Point p_health{106,172};

    GD::Point p_strength{106,225};
    GD::Point p_defense{106,257};
    GD::Point p_precision{106,289};
    GD::Point p_speed{106,321};
    GD::Point p_luck{106,353};
    GD::TrueColor color_white{0xFF, 0xFF, 0xFF};

    text = std::to_string(current_health)+ " / " +std::to_string(max_health);
    image.AddImageText(color_white,p_health,15,text,true);
    text = std::to_string(strength);
    image.AddImageText(color_white,p_strength,20,text,true);
    text = std::to_string(defense);
    image.AddImageText(color_white,p_defense,20,text,true);
    text = std::to_string(precision);
    image.AddImageText(color_white,p_precision,20,text,true);
    text = std::to_string(speed);
    image.AddImageText(color_white,p_speed,20,text,true);
    text = std::to_string(luck);
    image.AddImageText(color_white,p_luck,20,text,true);

    GD::TrueColor color{0x0,0x0,0x0};
    GD::Point lvl_forage{272,183};
    text = std::to_string(forage_lvl);
    image.AddImageText(color,lvl_forage,20,text,true);
    GD::Point xp_forage_rect_start(240 + (156 * forage_xp_percent),188);
    GD::Point xp_forage_rect_end(395,191);
    image.FilledRectangle(xp_forage_rect_start,xp_forage_rect_end,color.Int());
    std::cout << 156 * forage_xp_percent << std::endl;
    std::cout << xp_forage_rect_start.X() << std::endl;

    text = std::to_string(mining_lvl);
    GD::Point lvl_mining{272,235};
    image.AddImageText(color,lvl_mining,20,text,true);
    GD::Point xp_mining_rect_start(240+ (156 * mining_xp_percent),240);
    GD::Point xp_mining_rect_end(395,243);
    image.FilledRectangle(xp_mining_rect_start,xp_mining_rect_end,color.Int());

    text = std::to_string(combat_lvl);
    GD::Point lvl_combat{272,287};
    image.AddImageText(color,lvl_combat,20,text,true);
    GD::Point xp_combat_rect_start(240+ (156 * combat_xp_percent),292);
    GD::Point xp_combat_rect_end(395,295);
    image.FilledRectangle(xp_combat_rect_start,xp_combat_rect_end,color.Int());

    text = std::to_string(athletics_lvl);
    GD::Point lvl_athletics{272,341};
    image.AddImageText(color,lvl_athletics,20,text,true);
    GD::Point xp_athletics_rect_start(240+ (156 * athletics_xp_percent),346);
    GD::Point xp_athletics_rect_end(395,349);
    image.FilledRectangle(xp_athletics_rect_start,xp_athletics_rect_end,color.Int());
}

Renderer::PostRenderer::PostRenderer(int32_t capacity, int capacity_value, float filled_percent, int32_t gen_second, float gen_second_value, int32_t fortune, float fortune_value, std::string post_image) 
: ImageRenderer(image_path)
{
    GD::Point p_capacity{ 173 , 42 };
    GD::Point p_gen_second{ 173, 78};
    GD::Point p_fortune{ 173 , 114 };
    GD::TrueColor black{0x00,0x00,0x00};

    std::string text;
    //TODO:: FORMAT STRING
    text = std::to_string(capacity)+ "/" + std::to_string(capacity_value);
    image.AddImageText(black,p_capacity,15,text,true);
    text = std::to_string(gen_second)+ "/" + std::to_string(std::round(gen_second_value) /10);
    image.AddImageText(black,p_gen_second,15,text,true);
    text = std::to_string(fortune)+ "/" + std::to_string(std::round(fortune_value) /10);
    image.AddImageText(black,p_fortune,15,text,true);

    std::cout << post_image << std::endl;
    std::ifstream in{post_image, std::ios::binary};
    GD::Image post{in};
    GD::Point p_image{ 12 , 12 };
    GD::Size s;
    post.GetSize(s);
    image.Copy(post,p_image,GD::Point{0,0},s);

    //236
    GD::Point p_rectangle_start{ 136 + static_cast<int>(236 * filled_percent), 120 };
    GD::Point p_rectangle_end{ 371, 129};

    image.FilledRectangle(p_rectangle_start,p_rectangle_end,black.Int());


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