#include <core/game_manager.hpp>

PBLocationID gm::startLocation = PBLocationID::MAIN_BASE;

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
    if (!databaseID) 
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
    auto database_info = db_handler::FindPlayerCurrentInteraction(discord_id,user->GetCurrentPlayer(),location->GetDatabaseID(),databaseID.value());
    if (database_info) 
    {
        auto zone_access = static_cast<ZoneAccessInfo * const>(database_info->second.get());
        int unlocked_level = zone_access->GetUnlockedLvl();
        auto is_unlocked = location->GetZoneAccessUnlocked(interaction,unlocked_level);
        if (!is_unlocked.has_value()) return Errors::ILLEGAL_ACTION;
        if (is_unlocked.value()) 
        {
            PBLocationID next_loc = location->GetZoneAccessNextLoc(interaction).value();
            auto next_loc_info = GameMap::DCLMap::getInstance().GetLocation(next_loc);
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

gm::Errors gm::CanUnlock(uint64_t discord_id, int32_t interaction, std::string &output) 
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
    auto interaction_info = db_handler::FindPlayerCurrentInteraction(discord_id,user.value().GetCurrentPlayer(),location->GetDatabaseID(),interaction_db.value());
    if (!interaction_info.has_value()) return Errors::DATABASE_CONNECTION_ERROR;
    auto zone_access = static_cast<ZoneAccessInfo * const>(interaction_info->second.get());
    auto is_unlocked = location->GetZoneAccessUnlocked(interaction,zone_access->GetUnlockedLvl());
    if (!is_unlocked.has_value()) return Errors::ILLEGAL_ACTION;
    if (is_unlocked.value()) return Errors::INTERACTION_ALREADY_UNLOCKED;
    auto get_requirements = location->GetZoneAccessLevelRequirements(interaction,zone_access->GetUnlockedLvl());
    std::vector<Item> item_requirements;
    item_requirements.reserve(get_requirements.size());
    for (auto &req : get_requirements)
    {
        item_requirements.emplace_back(req.itemid(),req.quantity());
        auto name = PBItemEnum_Name(req.itemid());
        output += "    -> "+name+": "+std::to_string(req.quantity())+"\n";
    }

    auto items = db_handler::GetItems(discord_id,user.value().GetCurrentPlayer(),Item::RESOURCE_TYPE , item_requirements);
    if (item_requirements.size() > items.size()) return Errors::NOT_ENOUGH_RESOURCES;
    bool enough_resources = true;
    output += "-----------------\nYOU HAVE\n-----------------\n";
    for (int i = 0; i<items.size(); i++)
    {
        auto name = PBItemEnum_Name(items[i].GetItemId());
        output += "    -> "+name+": "+std::to_string(items[i].GetQuantity())+"\n";
        if (items[i].GetQuantity() < item_requirements[i].GetQuantity())
        {
            enough_resources = false;
        }
    }
    if (enough_resources) {
        return Errors::SUCCESS;
    } else {
        output += "NOT ENOUGH RESOURCES, CAN'T UNLOCK\n";
        return Errors::NOT_ENOUGH_RESOURCES;
    }
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
    auto interaction_info = db_handler::FindPlayerCurrentInteraction(discord_id,user.value().GetCurrentPlayer(),location->GetDatabaseID(),interaction_db.value());
    if (!interaction_info.has_value() || !interaction_info->second) return Errors::DATABASE_CONNECTION_ERROR;
    auto zone_access = static_cast<ZoneAccessInfo * const>(interaction_info->second.get());
    auto is_unlocked = location->GetZoneAccessUnlocked(interaction,zone_access->GetUnlockedLvl());
    if (!is_unlocked.has_value()) return Errors::ILLEGAL_ACTION;
    if (is_unlocked.value()) return Errors::INTERACTION_ALREADY_UNLOCKED;
    auto get_requirements = location->GetZoneAccessLevelRequirements(interaction,zone_access->GetUnlockedLvl());
    std::vector<Item> item_requirements;
    item_requirements.reserve(get_requirements.size());
    for (auto &req : get_requirements)
    {
        item_requirements.emplace_back(req.itemid(),req.quantity());
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
    db_handler::UnlockLocation(interaction_info.value().first,location->GetDatabaseID(),interaction_db.value());

    return Errors::SUCCESS;
}

gm::Errors gm::CollectPost(uint64_t discord_id, int32_t interaction, std::string &output) 
{
    output = "An Error ocurred, the post you are trying to collect was not found";
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return Errors::USER_NOT_FOUND;

    int location_id = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(location_id)) return Errors::GENERAL_ERROR;
    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(location_id));

    auto interaction_type = location->GetInteractionType(interaction);
    if (!interaction_type.has_value()) return Errors::INTERACTION_NOT_FOUND;
    if (interaction_type.value() != PBInteractionType::POST) return Errors::ILLEGAL_ACTION;

    auto interaction_db = location->GetInteractionDatabaseID(interaction).value();
    auto interaction_info = db_handler::FindPlayerCurrentInteraction(discord_id,user.value().GetCurrentPlayer(),location->GetDatabaseID(),interaction_db);
    if (interaction_info) 
    {
        auto *post_info = static_cast<PostInfo *>(interaction_info->second.get());
        bool update_time = true;
        std::vector<Item> items_result = location->CalculatePostRewards(interaction,post_info,interaction_info->first.GetStats(),interaction_info->first.GetSkills(), update_time);
        if (db_handler::CollectPost(interaction_info->first,interaction,post_info->GetResourceStored(),Item::RESOURCE_TYPE,items_result, update_time))
        {
            output = "\nSTORED: "+std::to_string(post_info->GetResourceStored());
            for (auto& res : items_result)
            {
                auto name = PBItemEnum_Name(res.GetItemId());
                output += "\n    -> "+name+" : "+ std::to_string(res.GetQuantity());
            }
            return Errors::SUCCESS;
        }   
    }
    return Errors::DATABASE_CONNECTION_ERROR;
}

gm::Errors gm::CanImprove(uint64_t discord_id, int32_t interaction_id, std::string upgrade_name, std::string &out) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return Errors::USER_NOT_FOUND;

    PBUpgradeType target_upgrade;
    if (!PBUpgradeType_Parse(upgrade_name,&target_upgrade)) return Errors::FORMAT_ERROR;

    int location_id = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(location_id)) return Errors::GENERAL_ERROR;
    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(location_id));

    auto interaction_type = location->GetInteractionType(interaction_id);
    if (!interaction_type.has_value()) return Errors::INTERACTION_NOT_FOUND;
    if (interaction_type.value() != PBInteractionType::POST) return Errors::ILLEGAL_ACTION;
    
    auto interaction_db = location->GetInteractionDatabaseID(interaction_id).value();
    auto interaction_info = db_handler::FindPlayerCurrentInteraction(discord_id,user.value().GetCurrentPlayer(),location->GetDatabaseID(),interaction_db);
    
    int current_level = 0;
    auto post_info = static_cast<PostInfo *>(interaction_info->second.get());
    switch (target_upgrade)
    {
    case PBUpgradeType::CAPACITY:
        current_level = post_info->GetCapacityLvl();
        break;
    case PBUpgradeType::GEN_SECOND:
        current_level = post_info->GetGenSecondLvl();
        break;
    case PBUpgradeType::FORTUNE:
        current_level = post_info->GetFortuneLvl();
        break;
    default:
        return Errors::GENERAL_ERROR;
    }

    auto requirements = location->GetPostLocationUpgradeRequirements(interaction_id, target_upgrade, current_level);
    if (requirements.size() <= 0) return Errors::INTERACTION_ALREADY_UNLOCKED;
    std::vector<Item> item_requirements;
    item_requirements.reserve(requirements.size());
    for (auto &req : requirements)
    {
        item_requirements.emplace_back(req.itemid(),req.quantity());
        auto name = PBItemEnum_Name(req.itemid());
        out += "    -> "+name+": "+std::to_string(req.quantity())+"\n";
    }

    auto items = db_handler::GetItems(discord_id,user.value().GetCurrentPlayer(),Item::RESOURCE_TYPE , item_requirements);
    bool enough_resources = true;
    if (item_requirements.size() > items.size()) enough_resources = false;
    else 
    {
        out += "-----------------\nYOU HAVE\n-----------------\n";
        for (int i = 0; i<items.size(); i++)
        {
            auto name = PBItemEnum_Name(items[i].GetItemId());
            out += "    -> "+name+": "+std::to_string(items[i].GetQuantity())+"\n";
            if (items[i].GetQuantity() < item_requirements[i].GetQuantity())
            {
                enough_resources = false;
            }   
        }
    }

    if (enough_resources) {
        return Errors::SUCCESS;
    } else {
        out += "NOT ENOUGH RESOURCES, CAN'T UNLOCK\n";
        return Errors::NOT_ENOUGH_RESOURCES;
    }
}

gm::Errors gm::ImprovePost(uint64_t discord_id, int32_t interaction_id, std::string upgrade_name)
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return Errors::USER_NOT_FOUND;

    PBUpgradeType target_upgrade;
    if (!PBUpgradeType_Parse(upgrade_name,&target_upgrade)) return Errors::FORMAT_ERROR;

    int location_id = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(location_id)) return Errors::GENERAL_ERROR;
    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(location_id));

    auto interaction_type = location->GetInteractionType(interaction_id);
    if (!interaction_type.has_value()) return Errors::INTERACTION_NOT_FOUND;
    if (interaction_type.value() != PBInteractionType::POST) return Errors::ILLEGAL_ACTION;
    
    auto interaction_db = location->GetInteractionDatabaseID(interaction_id).value();
    auto interaction_info = db_handler::FindPlayerCurrentInteraction(discord_id,user.value().GetCurrentPlayer(),location->GetDatabaseID(),interaction_db);
    
    int current_level = 0;
    auto post_info = static_cast<PostInfo *>(interaction_info->second.get());
    switch (target_upgrade)
    {
    case PBUpgradeType::CAPACITY:
        current_level = post_info->GetCapacityLvl();
        break;
    case PBUpgradeType::GEN_SECOND:
        current_level = post_info->GetGenSecondLvl();
        break;
    case PBUpgradeType::FORTUNE:
        current_level = post_info->GetFortuneLvl();
        break;
    default:
        return Errors::GENERAL_ERROR;
    }

    auto requirements = location->GetPostLocationUpgradeRequirements(interaction_id, target_upgrade, current_level);
    if (requirements.size() <= 0) return Errors::INTERACTION_ALREADY_UNLOCKED;
    std::vector<Item> item_requirements;
    item_requirements.reserve(requirements.size());
    for (auto &req : requirements)
    {
        item_requirements.emplace_back(req.itemid(),req.quantity());
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

    db_handler::ModifyItemsQuantity(discord_id,user.value().GetCurrentPlayer(),Item::RESOURCE_TYPE,item_requirements, true);
    db_handler::ImprovePost(interaction_info.value().first,interaction_id,upgrade_name);

    return Errors::SUCCESS;
    
}

std::unique_ptr<char, void(*)(char*)> gm::PhotoCurrentLocation(uint64_t discord_id, int *size) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};

    int location_id = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(location_id)) return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};
    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(location_id));

    Renderer::LocationRender location_img{location->GetImagePath()};
    auto interactions = db_handler::FindPlayerCurrentLocationInteractions(discord_id, user->GetCurrentPlayer(), location->GetDatabaseID());
    int interaction_count = location->GetInteractionSize();
    int posX = 0;
    int posY = 0;
    for (size_t i = 0; i < interaction_count; i++)
    {
        posX = location->GetInteractionPosX(i);
        posY = location->GetInteractionPosY(i);
        auto database_id = location->GetInteractionDatabaseID(i);
        if (database_id) 
        {
            auto curr_interaction = interactions->second.at(database_id.value()).get();
            switch (curr_interaction->GetType()) 
            {
                case InteractionType::ZONE_ACCESS:
                    {
                        auto zone_access = static_cast<ZoneAccessInfo*>(curr_interaction);
                        auto opt_bool = location->GetZoneAccessUnlocked(i,zone_access->GetUnlockedLvl());
                        if (opt_bool && !opt_bool.value()) 
                        {
                            location_img.AddInteraction(posX, posY, location->GetZoneAccessImage(i,0));
                        }
                        break;
                    }
                default:
                    break;
            }
        }
        
    }
    return location_img.RenderImage(size);
}

std::unique_ptr<char, void(*)(char*)> gm::Inventory(uint64_t discord_id, std::string group_type, int page_number, int *size, bool &is_last_page) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};

    auto item_data = db_handler::GetInventory(discord_id,user->GetCurrentPlayer(),group_type);
    std::string ui_path = "resources/assets/UI/" + group_type + ".png";
    Renderer::InventoryRender inventory_image{ui_path};
    
    auto& instance = GameMap::DCLMap::getInstance();
    
    is_last_page = ((item_data.size() / 8) + (item_data.size() % 8 == 0 ? 0 : 1)) == page_number + 1;
    if (item_data.size() == 0) is_last_page = true;
    for (size_t i = page_number, j = 0; i < item_data.size() && j < 8; i++, j++)
    {
        const std::string *path = instance.GetItemPath(item_data.at(i).GetItemId());
        const std::string *name = instance.GetItemName(item_data.at(i).GetItemId());
        inventory_image.AddItemToInventory(*path, *name, std::to_string(item_data.at(i).GetQuantity()));
    }

    
    return inventory_image.RenderImage(size);
}

std::unique_ptr<char, void(*)(char*)> gm::PhotoCurrentPost(uint64_t discord_id, int32_t interaction_id, std::string &output, int *size) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) 
    {
        output = "User could not be found, please initialize game with /start";
        return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};
    }

    int location_id = db_handler::CurrentPlayerLocation(discord_id,user->GetCurrentPlayer());
    if (!PBLocationID_IsValid(location_id)) 
    {
        output = "You are not in a valid location, how did you end up in here?";
        return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};
    } 

    auto location = GameMap::DCLMap::getInstance().GetLocation(static_cast<PBLocationID>(location_id));

    auto interaction_type = location->GetInteractionType(interaction_id);
    if (!interaction_type.has_value() || interaction_type.value() != PBInteractionType::POST)
    {
        output = "Post not found or target is not of type post. You can only get the info from posts!";
        return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};
    }

    auto location_db = location->GetDatabaseID();
    auto interaction_db = location->GetInteractionDatabaseID(interaction_id).value();
    auto player_interaction = db_handler::FindPlayerCurrentInteraction(discord_id,user->GetCurrentPlayer(),location_db,interaction_db);

    if (!player_interaction || !player_interaction->second) 
    {
        output = "Database error, please try again";
        return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};
    }

    auto post_info = static_cast<PostInfo *>(player_interaction->second.get());
    
    location->UpdatePostStored(interaction_id, post_info);
    
    auto cap_lvl = post_info->GetCapacityLvl();
    int cap_val = location->GetPostValueFromLvl(interaction_id, PBUpgradeType::CAPACITY, cap_lvl);
    auto curr_stored = post_info->GetResourceStored();
    auto filled_percent = curr_stored/cap_val;
    auto gen_sec_lvl = post_info->GetGenSecondLvl();
    auto gen_sec_val = location->GetPostValueFromLvl(interaction_id, PBUpgradeType::GEN_SECOND, gen_sec_lvl);
    auto fortune_lvl = post_info->GetFortuneLvl();
    auto fortune_val = location->GetPostValueFromLvl(interaction_id, PBUpgradeType::FORTUNE, fortune_lvl);
    int post_lvl = ( gen_sec_lvl + cap_lvl + fortune_lvl ) / 3;
    
    Renderer::PostRenderer renderer{cap_lvl,cap_val,filled_percent,gen_sec_lvl,gen_sec_val,fortune_lvl,fortune_val,location->GetPostImage(interaction_id, post_lvl)};
    return renderer.RenderImage(size);
}

std::unique_ptr<char, void(*)(char*)> gm::PlayerInfo(uint64_t discord_id, int *size) 
{
    auto user = db_handler::FindUserById(discord_id);
    if (!user) return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};

    auto player_data = db_handler::GetCurrentPlayer(discord_id, user->GetCurrentPlayer());
    if (!player_data) return std::unique_ptr<char, void(*)(char*)>{nullptr,nullptr};

    Skills *plyr_skills = player_data->GetSkills();
    Stats *plyr_stats = player_data->GetStats();
    float forage_xp_percent = static_cast<float>(plyr_skills->m_forage_xp) / GameLogic::CheckLevel(plyr_skills->m_forage_lvl);
    float mining_xp_percent = static_cast<float>(plyr_skills->m_mining_xp) / GameLogic::CheckLevel(plyr_skills->m_mining_lvl);
    float combat_xp_percent = static_cast<float>(plyr_skills->m_combat_xp) / GameLogic::CheckLevel(plyr_skills->m_combat_lvl);
    float athletics_xp_percent = static_cast<float>(plyr_skills->m_athletics_xp) / GameLogic::CheckLevel(plyr_skills->m_athletics_lvl);
    std::cout << forage_xp_percent << std::endl;
    std::cout << GameLogic::CheckLevel(plyr_skills->m_forage_lvl) << std::endl;
    Renderer::PlayerRenderer playerRenderer{
        plyr_stats->m_max_health, plyr_stats->m_current_health,
        plyr_stats->m_strength, plyr_stats->m_defense, 
        plyr_stats->m_precision, plyr_stats->m_speed,
        plyr_stats->m_luck,
        plyr_skills->m_forage_lvl,plyr_skills->m_forage_xp, forage_xp_percent,
        plyr_skills->m_mining_lvl, plyr_skills->m_mining_xp, mining_xp_percent,
        plyr_skills->m_combat_lvl, plyr_skills->m_combat_xp, combat_xp_percent,
        plyr_skills->m_athletics_lvl,plyr_skills->m_athletics_xp,athletics_xp_percent};

    return playerRenderer.RenderImage(size);
}