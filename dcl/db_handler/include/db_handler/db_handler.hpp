#pragma once
#include <db_instance.hpp>
//#include <db_access.hpp>
//#include <discorp_config.hpp>
#include <db_write.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/player.hpp>
//#include <db_query.hpp>
#include <db_handler/entities/interaction.hpp>
#include <mongocxx/pipeline.hpp>

namespace db 
{
    bool CreateGameTransaction(User user, Player player);

    std::unique_ptr<User> FindUserById(uint64_t id);

    std::unique_ptr<Player> FindPlayerCurrentLocationOnly(uint64_t id, uint8_t player_id);

    bool UnlockNewLocation(uint64_t id, uint8_t player_id, g_enums::GameLocations new_location);

    std::unique_ptr<Player> FindPlayerByIdAndCurrent(uint64_t discord_id, uint8_t player_id);

    //std::unique_ptr<std::vector<Location>> FindPlayerLocations(uint64_t discord_id, uint8_t player_id);

    bool UpdateCurrentLoc(uint64_t discord_id, uint8_t player_id, g_enums::GameLocations location);
}