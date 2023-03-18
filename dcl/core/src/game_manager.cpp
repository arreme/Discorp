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
    auto database_info = db_handler::FindPlayerCurrentInteraction(discord_id,user->GetCurrentPlayer(),databaseID.value());
    if (database_info) 
    {
        auto zone_access = static_cast<ZoneAccessInfo *>(database_info->second.get());
        int unlocked_level = zone_access->GetUnlockedLvl();
        if (location->GetZoneAccessUnlocked(interaction,unlocked_level)) 
        {
            PBLocationID next_loc = location->GetZoneAccessNextLoc(interaction).value();
            
            if (db_handler::PlayerFirstTimeToLocation(database_info->first,next_loc)) 
            {
                auto next_loc_info = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(next_loc));
                auto interaction_info = next_loc_info->LocationBuilder();
                db_handler::FillLocation(database_info->first,next_loc_info->GetDatabaseID(),interaction_info);
            }
            db_handler::GoToLocation(database_info->first,next_loc);
            return Errors::SUCCESS;
        }

        return Errors::LOCATION_LOCKED;
    }

    return Errors::DATABASE_CONNECTION_ERROR;
}