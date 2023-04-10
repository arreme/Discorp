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
    extern PBLocationID startLocation;

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

    Errors CanUnlock(uint64_t discord_id, int32_t interaction, std::string &output);
    
    Errors UnlockZone(uint64_t discord_id, int32_t interaction);

    Errors CollectPost(uint64_t discord_id, int32_t interaction, std::string &output);

    Errors CanImprove(uint64_t discord_id, int32_t interaction, std::string upgrade_name, std::string &out);

    Errors ImprovePost(uint64_t discord_id, int32_t interaction, std::string upgrade_name);



    std::unique_ptr<char, void(*)(char*)> PhotoCurrentLocation(uint64_t discord_id, int *size);

    std::unique_ptr<char, void(*)(char*)> Inventory(uint64_t discord_id, std::string group_type, int page_number, int *size, bool &is_last_page);

    std::unique_ptr<char, void(*)(char*)> PhotoCurrentPost(uint64_t discord_id, int32_t interaction_id, std::string &output, int *size);

    std::unique_ptr<char, void(*)(char*)> PlayerInfo(uint64_t discord_id, int *size);
}