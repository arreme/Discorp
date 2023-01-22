#pragma once

namespace g_enums 
{
    enum class GameLocations 
    {
        ERROR_LOCATION = -1,
        MAIN_BASE = 0,
        FOREST = 1,
        TOWN = 2,
        INTERNET = 3,
        MINES_1 = 4,
    };

    enum class Errors 
    {
        SUCCESS = 0,
        DUPLICATED_PK_ERROR = -1,
        FORMAT_ERROR = -2,
        GENERAL_ERROR = -3,
        DATABASE_CONNECTION_ERROR = -4,
    };
}

