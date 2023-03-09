#pragma once
#include <iostream>
#include <util/game_constants.hpp>
#include <db_handler/db_handler.hpp>
#include <db_handler/entities/player.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/interaction.hpp>

using namespace g_enums;

namespace gm 
{
    Errors CreateGame(uint64_t discord_id, std::string user_name);

    //Errors DeleteGame(uint64_t discord_id);

    Errors SetActivePlayer(uint64_t discord_id, uint8_t active_player);

    Errors GoToZone(uint64_t discord_id, g_enums::GameLocations location);

    //Errors PhotoCurrentLocation(uint64_t discord_id);
}