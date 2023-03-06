#pragma once
#include <vector>
#include <iostream>
#include <game_constants.hpp>
#include <db_handler.hpp>

using namespace g_enums;

namespace ga 
{
    Errors ActionMine(uint64_t discord_id, uint8_t target);
}