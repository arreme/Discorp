#include <resource_loader.hpp>

bsoncxx::document::value utils::LoadLocationInfo(g_enums::GameLocations loc) 
{
    std::string path = "resources/data/locations";
    auto val = std::filesystem::directory_iterator(path);
    int current = 0;
    int target = static_cast<int>(loc);
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        if (current == target)
        {
            std::ifstream t(entry.path());
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            return bsoncxx::from_json(str);
        }
        current++;
    }
    return bsoncxx::from_json({});
};

bsoncxx::document::value utils::LoadPostInfo(std::string fileName) 
{
    std::string path = "resources/data/posts/"+fileName;
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return bsoncxx::from_json(str);
};

assetPointer utils::ImageLoader(std::string path)
{
    std::function<void(gdImage *)> func = [](gdImage *img) { gdImageDestroy(img); };
    FILE *fp;
    gdImagePtr im;
    const char *name = path.c_str();
    fp = fopen(name, "rb");
    
    if (!fp) 
    {
        fprintf(stderr, "Can't open jpeg file\n");
        return assetPointer(nullptr,func);
    }

    im = gdImageCreateFromPng(fp);
    fclose(fp);

    return assetPointer(im,func);
}