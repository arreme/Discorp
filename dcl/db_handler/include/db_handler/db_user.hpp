#pragma once
#include <core/pb/player.pb.h>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <chrono>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/string/to_string.hpp>

using namespace bsoncxx::builder::basic;
using namespace google::protobuf;
using namespace bsoncxx::types;
using namespace google::protobuf::util;

namespace db_handler 
{
    class DBUserHandler 
    {
    private:
        PBUser *m_user;

        bsoncxx::document::value SkillsToBson(const PBSkills &skills);

        void BsonToSkills(PBSkills *skills, bsoncxx::document::view doc);

        bsoncxx::document::value StatsToBson(const PBStats &stats);

        void BsonToStats(PBStats *stats, bsoncxx::document::view doc);

        bsoncxx::document::value EquipationToBson(const PBEquipation &equipation);

        void BsonToEquipation(PBEquipation *equipation, bsoncxx::document::view doc);

        bsoncxx::document::value PlayerToBson(const PBPlayer &player);

        void BsonToPlayer(PBPlayer *player, bsoncxx::document::view doc);

    public:
        DBUserHandler(PBUser *user);
        /**
         * TESTED
         * 
         * Inserts an User using the m_user attribute
        */
        bool InsertUser(db::Transaction *t = nullptr) noexcept;
        /**
         * TESTED
         * 
         * Finds Current Player using user discord id
        */
        bool FindUserCurrentPlayer() noexcept;
    };

    
}


