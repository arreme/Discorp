#pragma once
#include <iostream>
#include <bsoncxx/json.hpp>
#include <filesystem>
#include <functional>
#include <fstream>
#include "gd.h"
#include <stdio.h>
#include <stdlib.h>

typedef std::unique_ptr<gdImage,std::function<void(gdImage *)>> assetPointer;

namespace utils 
{
    bsoncxx::document::value LoadLocationInfo(uint32_t loc);

    bsoncxx::document::value LoadPostInfo(std::string fileName);

    assetPointer ImageLoader(std::string path);
}