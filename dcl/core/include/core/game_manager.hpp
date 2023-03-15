#pragma once
#include <iostream>
#include <db_handler/db_handler.hpp>
#include <db_handler/entities/player.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/interaction.hpp>

namespace gm 
{
    enum class Errors 
    {
        SUCCESS = 0,
        DUPLICATED_PK_ERROR = -1,
        FORMAT_ERROR = -2,
        GENERAL_ERROR = -3,
        DATABASE_CONNECTION_ERROR = -4,
        USER_NOT_FOUND = -5,
        ILLEGAL_ACTION = -6,
    };

    Errors CreateGame(uint64_t discord_id, std::string user_name);

    //Errors DeleteGame(uint64_t discord_id);

    Errors SetActivePlayer(uint64_t discord_id, uint8_t active_player);

    Errors GoToZone(uint64_t discord_id, int32_t location);

    //Errors PhotoCurrentLocation(uint64_t discord_id);
}