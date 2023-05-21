#pragma once
#include <core/pb/combat.pb.h>
#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/string/to_string.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

namespace db_handler 
{
    class DBCombatHandler 
    {
    private:
        PBCombat *m_combat_db;

        bsoncxx::document::value SkillsToBson(const PBSkills &skills);

        void BsonToSkills(PBSkills *skills, bsoncxx::document::view doc);

        bsoncxx::document::value StatsToBson(const PBStats &stats);

        void BsonToStats(PBStats *stats, bsoncxx::document::view doc);

        bsoncxx::document::value EquipationToBson(const PBEquipation &equipation);

        void BsonToEquipation(PBEquipation *equipation, bsoncxx::document::view doc);

        bsoncxx::document::value PlayerToBson(const PBPlayer &player);

        void BsonToPlayer(PBPlayer *player, bsoncxx::document::view doc);

    public:
        DBCombatHandler(PBCombat *combat_db) : m_combat_db(combat_db) {};
        
        bool FindCurrentCombat();

        bool InsertNewCombat();

        bool UpdateCombatForPlayer(bool starting_player, bool opponent_player);

        bool DeleteCombat();
    };
}