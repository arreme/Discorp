#pragma once
#include <iostream>
#include <bsoncxx/json.hpp>
#include <filesystem>
#include <fstream>
#include <game_constants.hpp>
#include "gd.h"
#include <stdio.h>
#include <stdlib.h>

namespace utils 
{
    bsoncxx::document::value LoadLocationInfo(g_enums::GameLocations loc);

    bsoncxx::document::value LoadPostInfo(std::string fileName);

    gdImagePtr ImageLoader(std::string path);
}