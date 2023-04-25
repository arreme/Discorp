#pragma once
#include <ostream>
#include <core/pb/player.pb.h>
#include <dpp.h>

class BaseRequest 
{
private:
    bool user_created = false;
public:
    BaseRequest(uint64_t discord_id) 
    {
        //Check If user already registerered
    };
};

class CreateGameRequest
{
public: 
    CreateGameRequest(uint64_t discord_id, std::string user_name)
    {

    }

    bool FillResponse(dpp::message &m) 
    {

    }
};