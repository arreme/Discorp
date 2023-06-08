#include <core/game_logic.hpp>

int GameLogic::CheckLevel(int lvl) 
{
    float exponent = 1.2f;
    float baseXP = 20;
    return (baseXP * (pow(lvl,exponent)))+1;
}

int GameLogic::CalculateModifier(PBPlayerSkills id, const PBPlayer &player) 
{
    switch (id)
    {
    case PBPlayerSkills::FORAGE:
        return player.skills().foraging_lvl() + player.stats().speed();
    case PBPlayerSkills::MINING:
        return player.skills().mining_lvl() + player.stats().precision();
    case PBPlayerSkills::COMBAT:
        return player.skills().combat_lvl() + player.stats().strength();
    case PBPlayerSkills::ATHLETICS:
        return player.skills().athletics_lvl() + player.stats().speed();
    default:
        break;
    }
    return 0;
}

void GameLogic::CalculateLevel(PBPlayerSkills id, int gathered, int resourceXp, PBPlayer *player_db) 
{
    auto requiredXP = 0;
    auto skills = player_db->mutable_skills();
    switch (id)
    {
    case PBPlayerSkills::FORAGE:
        skills->set_foraging_xp(skills->foraging_xp() + (gathered * resourceXp));
        requiredXP = GameLogic::CheckLevel(skills->foraging_lvl());
        if (skills->foraging_xp() >= requiredXP) 
        {
            skills->set_foraging_xp(skills->foraging_xp() - requiredXP);
            skills->set_foraging_lvl(skills->foraging_lvl() + 1);
        }
        break;
    
    case PBPlayerSkills::MINING:
        skills->set_mining_xp(skills->mining_xp() + (gathered * resourceXp));
        requiredXP = GameLogic::CheckLevel(skills->mining_lvl());
        if (skills->mining_xp() >= requiredXP) 
        {
            skills->set_mining_xp(skills->mining_xp() - requiredXP);
            skills->set_mining_lvl(skills->mining_lvl() + 1);
        }
        break;
    case PBPlayerSkills::ATHLETICS:
        skills->set_athletics_xp(skills->athletics_xp() + (gathered * resourceXp));
        requiredXP = GameLogic::CheckLevel(skills->athletics_lvl());
        if (skills->athletics_xp() >= requiredXP) 
        {
            skills->set_athletics_xp(skills->athletics_xp() - requiredXP);
            skills->set_athletics_lvl(skills->athletics_lvl() + 1);
        }
        break;
    case PBPlayerSkills::COMBAT:
        skills->set_combat_xp(skills->combat_xp() + (gathered * resourceXp));
        requiredXP = GameLogic::CheckLevel(skills->combat_lvl());
        if (skills->combat_xp() >= requiredXP) 
        {
            skills->set_combat_xp(skills->combat_xp() - requiredXP);
            skills->set_combat_lvl(skills->combat_lvl() + 1);
        }
        break;
    default:
        break;
    }
}