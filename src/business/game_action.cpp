#include <game_action.hpp>

Errors ga::ActionMine(uint64_t discord_id, uint8_t target) 
{
    auto result = db::FindPlayerUserAndLocation(discord_id);
    auto user = std::get<User>(result);
    return Errors::SUCCESS;
}