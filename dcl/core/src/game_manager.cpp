#include <core/game_manager.hpp>

Errors gm::CreateGame(uint64_t discord_id, std::string user_name) 
{
    User user{discord_id, user_name};
    Player player{discord_id, 1};

    std::vector<InteractionInfo *> info; //Get Info protobuf 

    if (!db_handler::RegisterPlayerToDatabase(user, player, info)) 
        return Errors::DUPLICATED_PK_ERROR;

    return Errors::SUCCESS;
}