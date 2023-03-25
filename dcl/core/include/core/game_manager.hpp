#pragma once
#include <iostream>
#include <db_handler/db_handler.hpp>
#include <db_handler/entities/player.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/interaction.hpp>
#include <core/dcl_map.hpp>
#include <img/renderer.hpp>

namespace gm 
{
    PBLocationID startLocation = PBLocationID::MAIN_BASE;

    enum class Errors 
    {
        SUCCESS = 0,
        DUPLICATED_PK_ERROR = -1,
        FORMAT_ERROR = -2,
        GENERAL_ERROR = -3,
        DATABASE_CONNECTION_ERROR = -4,
        USER_NOT_FOUND = -5,
        ILLEGAL_ACTION = -6,
        FORBIDDEN_ACTION = -7,
        INTERACTION_NOT_FOUND = -8,
        LOCATION_LOCKED = -9,
        NOT_ENOUGH_RESOURCES = -10,
        INTERACTION_ALREADY_UNLOCKED = -11
    };

    Errors CreateGame(uint64_t discord_id, std::string user_name);

    //Errors DeleteGame(uint64_t discord_id);

    Errors SetActivePlayer(uint64_t discord_id, uint8_t active_player);

    Errors GoToZone(uint64_t discord_id, int32_t interaction);
    
    Errors UnlockZone(uint64_t discord_id, int32_t interaction);

    Errors CollectPost(uint64_t discord_id, int32_t interaction);

    std::unique_ptr<char, void(*)(char*)> PhotoCurrentLocation(uint64_t discord_id, int *size);
}