#pragma once
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <db_handler/entities/user.hpp>
#include <db_handler/entities/interaction.hpp>
#include <db_handler/entities/player.hpp>
#include <db_handler/entities/inventory.hpp>

namespace db_handler
{
    typedef std::pair<Player,std::vector<std::unique_ptr<InteractionInfo>>> playerInteractions;

    /**
     * Something
    */
    bool ChangeActivePlayer(uint64_t discord_id, int32_t newPlayerSlot);

    bool RegisterPlayerToDatabase(User &user, Player &player, std::vector<std::reference_wrapper<InteractionInfo>> &info);

    std::optional<User> FindUserById(uint64_t discord_id);
    /**
     * Sets current location atribute to the desired
    */
    bool GoToLocation(Player &player, int32_t new_location);

    /**
     * Sets specific zone access interaction to true
    */
    bool FillLocation(Player &player, int32_t unlocked_location, std::vector<std::reference_wrapper<InteractionInfo>> &info);
    
    bool CollectPost(Player &player, int32_t interaction_id);

    bool ImprovePost(Player &player, int32_t interaction_id, std::string_view update_name);

    std::optional<playerInteractions> FindPlayerCurrentLocationInteractions(uint64_t discord_id, int32_t player_id);

    std::optional<std::pair<Player,std::unique_ptr<InteractionInfo>>> FindPlayerCurrentInteraction(uint64_t discord_id, int32_t player_id, int32_t interaction_id);

    bool PlayerFirstTimeToLocation(Player &player,int32_t location_id);

    int CurrentPlayerLocation(uint64_t discord_id, int32_t player_id);

    bsoncxx::array::value FillInteracionsDocument(std::vector<std::reference_wrapper<InteractionInfo>> &info);

    mongocxx::pipeline PlayerCurrentLocationInteractions_Pipeline(uint64_t discord_id, int32_t player_id);

    mongocxx::pipeline PlayerCurrentInteraction_Pipeline(uint64_t discord_id, int32_t player_id, int32_t interaction_id);

    //Inventory

    bool ModifyItemQuantity(uint64_t discord_id, int32_t player_id, std::string category, int item_id, int quantity);

    std::optional<Item> GetItem(uint64_t discord_id, int32_t player_id, std::string category, int item_id);

    std::vector<Item> GetInventory(uint64_t discord_id, int32_t player_id, std::string category, int current_page);
}