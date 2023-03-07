#pragma once
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <db_handler/entities/interaction.hpp>
#include <db_handler/entities/player.hpp>

namespace db_handler
{
    typedef std::pair<Player,std::vector<std::unique_ptr<InteractionInfo>>> playerInteractions;

    bool RegisterPlayerToDatabase(Player &player, std::vector<InteractionInfo *> &info);

    //bool NewLocationUnlocked(std::vector<InteractionInfo *> &info);

    //FindPlayerCurrentInteraction();

    std::optional<playerInteractions> FindPlayerCurrentLocationInteractions(uint64_t discord_id, int32_t player_id);

    std::optional<std::pair<Player,std::unique_ptr<InteractionInfo>>> FindPlayerCurrentInteraction(uint64_t discord_id, int32_t player_id, int32_t interaction_id);

    mongocxx::pipeline PlayerCurrentLocationInteractions_Pipeline(uint64_t discord_id, int32_t player_id);

    mongocxx::pipeline PlayerCurrentInteraction_Pipeline(uint64_t discord_id, int32_t player_id, int32_t interaction_id);
}