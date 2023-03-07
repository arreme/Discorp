#pragma once
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <db_handler/entities/interaction.hpp>
#include <db_handler/entities/player.hpp>

namespace db_handler
{
    bool RegisterPlayerToDatabase(Player &player, std::vector<InteractionInfo *> &info);

    //bool NewLocationUnlocked(std::vector<InteractionInfo *> &info);
}