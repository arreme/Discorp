#pragma once
#include <core/pb/map.pb.h>
#include <math.h>

namespace GameLogic 
{
    int CheckLevel(int lvl);
    int CalculateModifier(PBPlayerSkills id, const PBPlayer &player);
    void CalculateLevel(PBPlayerSkills id, int gathered, int resourceXp, PBPlayer *player_db);
}