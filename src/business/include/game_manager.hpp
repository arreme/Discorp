#pragma once
#include <vector>
#include <iostream>

namespace gm 
{
    typedef enum GM_ERR_CODE
    {
        SUCCESS = 0,
        USER_ALREADY_ADDED = -1,
        FORMAT_ERROR = -2,
    } gm_err_code;

    gm_err_code CreateGame(uint64_t discord_id, std::string user_name);
}