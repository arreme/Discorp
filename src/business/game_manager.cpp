#include <game_manager.hpp>
#include <user.hpp>
#include <db_instance.hpp>
#include <discorp_config.hpp>

using namespace gm;


g_enums::Errors CreateGame(uint64_t discord_id, std::string user_name) 
{
    //Find user if user can't be found create user and initialize all game
    
    //User could be found, creating a new player if there are enough slots

    //User could not be found, initializing brand new game
    User user = User(discord_id,user_name);
    

    return g_enums::Errors::SUCCESS;
}

g_enums::Errors DeleteGame(uint64_t discord_id) 
{
    return g_enums::Errors::SUCCESS;
}

