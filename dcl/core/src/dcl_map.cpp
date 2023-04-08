#include <core/dcl_map.hpp>

std::map<PBLocationID, GameMap::DCLLocation> GameMap::DCLMap::m_locations;

std::map<PBItemType,PBItemsDict> GameMap::DCLMap::item_data;

PostInfo GameMap::InteractionTemplates::post;

ZoneAccessInfo GameMap::InteractionTemplates::zone;



int GameLogic::CheckLevel(int lvl) 
{
    float exponent = 1.2f;
    float baseXP = 20;
    return baseXP * (pow(lvl,exponent));
}

int GameLogic::CalculateModifier(PBPlayerSkills id, Stats *const plyr_stats, Skills *const plyr_skills) 
{
    switch (id)
    {
    case PBPlayerSkills::FORAGE:
        return plyr_skills->m_forage_lvl + plyr_stats->m_speed;
    case PBPlayerSkills::MINING:
        return plyr_skills->m_mining_lvl + plyr_stats->m_precision;
    default:
        break;
    }
    return 0;
}

void GameLogic::CalculateLevel(PBPlayerSkills id, int gathered, int resourceXp, Skills *const plyr_skills) 
{
    auto requiredXP = 0;
    switch (id)
    {
    case PBPlayerSkills::FORAGE:
        plyr_skills->m_forage_xp += gathered * resourceXp;
        requiredXP = GameLogic::CheckLevel(plyr_skills->m_forage_lvl);
        if (plyr_skills->m_forage_xp >= requiredXP) 
        {
            plyr_skills->m_forage_xp -= requiredXP;
            plyr_skills->m_forage_lvl += 1;
        }
        break;
    
    case PBPlayerSkills::MINING:
        plyr_skills->m_mining_xp += gathered * resourceXp;
        requiredXP = GameLogic::CheckLevel(plyr_skills->m_mining_lvl);
        if (plyr_skills->m_mining_xp >= requiredXP) 
        {
            plyr_skills->m_mining_xp -= requiredXP;
            plyr_skills->m_mining_lvl += 1;
        }
        break;
    case PBPlayerSkills::ATHLETICS:
        plyr_skills->m_athletics_xp += gathered * resourceXp;
        requiredXP = GameLogic::CheckLevel(plyr_skills->m_athletics_lvl);
        if (plyr_skills->m_athletics_xp >= requiredXP) 
        {
            plyr_skills->m_athletics_xp -= requiredXP;
            plyr_skills->m_athletics_lvl += 1;
        }
        break;
    default:
        break;
    }
}