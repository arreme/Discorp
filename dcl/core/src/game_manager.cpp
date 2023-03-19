#include <core/game_manager.hpp>

gm::Errors gm::CreateGame(uint64_t discord_id, std::string user_name) 
{
    User user{discord_id, user_name};
    Player player{discord_id, 1,startLocation};

    auto info = GameMap::DCLMap::getInstance().GetLocation(startLocation)->LocationBuilder();

    if (!db_handler::RegisterPlayerToDatabase(user, player, info)) 
        return Errors::DUPLICATED_PK_ERROR;

    return Errors::SUCCESS;
}

gm::Errors gm::GoToZone(uint64_t discord_id, int32_t interaction) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return Errors::USER_NOT_FOUND;

    int result = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(result)) return Errors::GENERAL_ERROR;
    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(result));
    
    auto interaction_type = location->GetInteractionType(interaction);
    if (!interaction_type.has_value()) return Errors::INTERACTION_NOT_FOUND;
    if (interaction_type.value() != PBInteractionType::ZONE_ACCESS) return Errors::ILLEGAL_ACTION;
    auto databaseID = location->GetInteractionDatabaseID(interaction);
    if (databaseID == -1) 
    {
        PBLocationID next_loc = location->GetZoneAccessNextLoc(interaction).value();
        auto next_loc_info = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(next_loc));
        if (!next_loc_info) return Errors::ILLEGAL_ACTION;
        if (next_loc_info->GetDatabaseID() != -1 && db_handler::PlayerFirstTimeToLocation(discord_id,user->GetCurrentPlayer(),next_loc)) 
        {
            auto interaction_info = next_loc_info->LocationBuilder();
            db_handler::FillLocation(discord_id,user->GetCurrentPlayer(),next_loc_info->GetDatabaseID(),interaction_info);
        }
        db_handler::GoToLocation(discord_id,user->GetCurrentPlayer(),next_loc);
        return Errors::SUCCESS;
    }
    auto database_info = db_handler::FindPlayerCurrentInteraction(discord_id,user->GetCurrentPlayer(),databaseID.value());
    if (database_info) 
    {
        auto zone_access = static_cast<ZoneAccessInfo *>(database_info->second.get());
        int unlocked_level = zone_access->GetUnlockedLvl();
        auto is_unlocked = location->GetZoneAccessUnlocked(interaction,unlocked_level);
        if (!is_unlocked.has_value()) return Errors::ILLEGAL_ACTION;
        if (is_unlocked.value()) 
        {
            PBLocationID next_loc = location->GetZoneAccessNextLoc(interaction).value();
            auto next_loc_info = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(next_loc));
            if (!next_loc_info) return Errors::ILLEGAL_ACTION;
            if (next_loc_info->GetDatabaseID() != -1 && db_handler::PlayerFirstTimeToLocation(discord_id,user->GetCurrentPlayer(),next_loc)) 
            {
                auto interaction_info = next_loc_info->LocationBuilder();
                db_handler::FillLocation(discord_id,user->GetCurrentPlayer(),next_loc_info->GetDatabaseID(),interaction_info);
            }
            db_handler::GoToLocation(discord_id,user->GetCurrentPlayer(),next_loc);
            return Errors::SUCCESS;
        }

        return Errors::LOCATION_LOCKED;
    }

    return Errors::DATABASE_CONNECTION_ERROR;
}

gm::Errors gm::UnlockZone(uint64_t discord_id, int32_t interaction) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return Errors::USER_NOT_FOUND;

    int result = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(result)) return Errors::GENERAL_ERROR;
    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(result));

    auto interaction_type = location->GetInteractionType(interaction);
    if (!interaction_type.has_value()) return Errors::INTERACTION_NOT_FOUND;
    if (interaction_type.value() != PBInteractionType::ZONE_ACCESS) return Errors::ILLEGAL_ACTION;

    auto interaction_db = location->GetInteractionDatabaseID(interaction);
    if (!interaction_db.has_value()) return Errors::INTERACTION_ALREADY_UNLOCKED;
    auto interaction_info = db_handler::FindPlayerCurrentInteraction(discord_id,user.value().GetCurrentPlayer(),interaction_db.value());
    if (!interaction_info.has_value()) return Errors::DATABASE_CONNECTION_ERROR;
    auto zone_access = static_cast<ZoneAccessInfo *>(interaction_info->second.get());
    auto is_unlocked = location->GetZoneAccessUnlocked(interaction,zone_access->GetUnlockedLvl());
    if (!is_unlocked.has_value()) return Errors::ILLEGAL_ACTION;
    if (is_unlocked.value()) return Errors::INTERACTION_ALREADY_UNLOCKED;
    auto get_requirements = location->GetZoneAccessLevelRequirements(interaction,zone_access->GetUnlockedLvl());
    std::vector<Item> item_requirements;
    for (auto &req : get_requirements)
    {
        item_requirements.push_back(Item{req.itemid(),req.quantity()});
    }

    auto items = db_handler::GetItems(discord_id,user.value().GetCurrentPlayer(),Item::RESOURCE_TYPE , item_requirements);
    if (item_requirements.size() > items.size()) return Errors::NOT_ENOUGH_RESOURCES;
    
    for (int i = 0; i<items.size(); i++)
    {

        if (items[i].GetQuantity() < item_requirements[i].GetQuantity())
        {
            return Errors::NOT_ENOUGH_RESOURCES;
        }
        
    }
    
    //TODO: Transaction? Make it one call
    db_handler::ModifyItemsQuantity(discord_id,user.value().GetCurrentPlayer(),Item::RESOURCE_TYPE,item_requirements, true);
    db_handler::UnlockLocation(interaction_info.value().first,result,interaction_db.value());

    return Errors::SUCCESS;
}

gm::Errors gm::CollectPost(uint64_t discord_id, int32_t interaction) 
{
    
}