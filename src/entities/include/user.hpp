#include <iostream>

class User 
{
private:
    uint64_t discord_id;
    std::string user_name;
    uint8_t strength;
    uint16_t health;

public:

    User(uint64_t id, std::string user_name);

    uint64_t GetId();

    std::string GetUserName();

};