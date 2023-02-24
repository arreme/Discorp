#pragma once
#include <iostream>
#include <bsoncxx/json.hpp>
#include <filesystem>
#include <fstream>
#include <game_constants.hpp>
#include "gd.h"
#include <stdio.h>
#include <stdlib.h>

typedef std::unique_ptr<gdImage,std::function<void(gdImage *)>> assetPointer;

namespace utils 
{
    bsoncxx::document::value LoadLocationInfo(g_enums::GameLocations loc);

    bsoncxx::document::value LoadPostInfo(std::string fileName);

    assetPointer ImageLoader(std::string path);
}