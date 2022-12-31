#pragma once
#include <vector>
#include <iostream>

namespace gm 
{
    typedef enum
    {
        SUCCESS = 0,
        USER_ALREADY_ADDED = -1,
        FORMAT_ERROR = -2,
        GENERAL_ERROR = -3
    } gm_err_code;

    gm_err_code CreateGame(uint64_t discord_id, std::string user_name);

    gm_err_code DeleteGame(uint64_t discord_id);
}