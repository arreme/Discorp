#pragma once
#include <vector>
#include <iostream>
#include <game_constants.hpp>

namespace gm 
{
    g_enums::Errors CreateGame(uint64_t discord_id, std::string user_name);

    g_enums::Errors DeleteGame(uint64_t discord_id);
}