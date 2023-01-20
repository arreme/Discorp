#include <game_manager.hpp>

using namespace g_enums;

Errors gm::CreateGame(uint64_t discord_id, std::string user_name) 
{
    //Find user if user can't be found create user and initialize all game
    auto usr_res = db::FindUserById(discord_id);
    if(usr_res) 
    {
        //TODO: User could be found, creating a new player if there are enough slots
        
        return Errors::DUPLICATED_PK_ERROR;
    }
    
    //User could not be found, initializing brand new game
    User user = User(discord_id,user_name);
    Player player = Player(discord_id,0);
    if(db::CreateGameTransaction(user,player)) 
    {
        return Errors::SUCCESS;
    }

    return Errors::GENERAL_ERROR;
}

Errors gm::DeleteGame(uint64_t discord_id) 
{
    return Errors::SUCCESS;
}

Errors gm::SetActivePlayer(uint64_t discord_id, uint8_t active_player) 
{
    return Errors::SUCCESS;
}

