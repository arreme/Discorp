#pragma once
#include <ostream>
#include <core/pb/player.pb.h>

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

class CreateGame : public BaseRequest
{
public: 
    CreateGame(uint64_t discord_id, std::string user_name):
    BaseRequest(discord_id)
    {

    }

    bool GetResponse() 
    {

    }
};