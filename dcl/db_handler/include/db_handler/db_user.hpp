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

        bsoncxx::document::value SkillsToBson(const PBSkills &skills) 
        {
            bsoncxx::builder::basic::document doc{};
            doc.append(kvp("foraging_lvl",b_int32{static_cast<int32_t>(skills.foraging_lvl())}));
            doc.append(kvp("foraging_xp",b_int64{static_cast<int64_t>(skills.foraging_xp())}));
            doc.append(kvp("mining_lvl",b_int32{static_cast<int32_t>(skills.mining_lvl())}));
            doc.append(kvp("mining_xp",b_int64{static_cast<int64_t>(skills.mining_xp())}));
            doc.append(kvp("combat_lvl",b_int32{static_cast<int32_t>(skills.combat_lvl())}));
            doc.append(kvp("combat_xp",b_int64{static_cast<int64_t>(skills.combat_xp())}));
            doc.append(kvp("athletics_lvl",b_int32{static_cast<int32_t>(skills.athletics_lvl())}));
            doc.append(kvp("athletics_xp",b_int64{static_cast<int64_t>(skills.athletics_xp())}));
            return doc.extract();
        };

        bsoncxx::document::value StatsToBson(const PBStats &stats) 
        {
            bsoncxx::builder::basic::document doc{};
            doc.append(kvp("max_health",b_int32{static_cast<int32_t>(stats.max_health())}));
            doc.append(kvp("current_health",b_int32{static_cast<int32_t>(stats.current_health())}));
            doc.append(kvp("strength",b_int32{stats.strength()}));
            doc.append(kvp("defense",b_int32{stats.defense()}));
            doc.append(kvp("precision",b_int32{stats.precision()}));
            doc.append(kvp("speed",b_int32{stats.speed()}));
            doc.append(kvp("luck",b_int32{stats.luck()}));
            return doc.extract();
        };

        bsoncxx::document::value EquipationToBson(const PBEquipation &equipation) 
        {
            bsoncxx::builder::basic::document doc{};
            doc.append(kvp("armor_head",b_int32{static_cast<int32_t>(equipation.armor_head())}));
            doc.append(kvp("armor_chest",b_int32{static_cast<int32_t>(equipation.armor_chest())}));
            doc.append(kvp("armor_leggins",b_int32{static_cast<int32_t>(equipation.armor_leggings())}));
            doc.append(kvp("armor_boots",b_int32{static_cast<int32_t>(equipation.armor_boots())}));
            doc.append(kvp("weapon",b_int32{static_cast<int32_t>(equipation.weapon())}));
            doc.append(kvp("tool",b_int32{static_cast<int32_t>(equipation.tool())}));

            bsoncxx::builder::basic::array attacks{};
            for (auto const& attack : equipation.attacks())
            {
                attacks.append(attack);
            }
            doc.append(kvp("attacks",b_array{attacks}));
            return doc.extract();
        };

    public:
        DBUserHandler(PBUser *user)
        : m_user(user)  
        {};

        bool InsertUser(db::Transaction *t = nullptr) 
        {
            if (!m_user) return false;
            bsoncxx::builder::basic::document doc{};
            doc.append(kvp("discord_id",b_int64{static_cast<int64>(m_user->discord_id())}));
            doc.append(kvp("user_name",b_string{m_user->user_name()}));
            doc.append(kvp("current_player_id",b_int32{m_user->current_player_id()}));
            doc.append(kvp("last_online",b_date{std::chrono::seconds(TimeUtil::TimestampToSeconds(m_user->last_online()))}));
            
            auto player = m_user->players(0);

            bsoncxx::builder::basic::array players{};
            for (auto const& player : m_user->players())
            {
                players.append(make_document(
                    kvp("guild_id",b_int64{static_cast<int64>(player.guild_id())}),
                    kvp("current_location",b_int32{static_cast<int32>(player.current_location())}),
                    kvp("stats",StatsToBson(player.stats())),
                    kvp("skills",SkillsToBson(player.skills())),
                    kvp("equipation",EquipationToBson(player.equipation()))
                ));
            }
            doc.append(kvp("players",b_array{players}));

            if (t) 
            {
                t->AddOperation(std::make_unique<db::InsertOneOperation>("users",doc.extract()));
                return true;
            } else 
            {
                db::InsertOneOperation insert_op{"users",doc.extract()};
                insert_op.ExecuteOperation();
                return insert_op.GetState() == db::OperationState::SUCCESS;
            }
            
        }

        bool FindUserCurrentPlayer() 
        {
            if (!m_user) return false;
            bsoncxx::builder::basic::document doc{};
            mongocxx::pipeline p;
            p.match(make_document(
                kvp("discord_id",b_int64{static_cast<int64>(m_user->discord_id())}),
                kvp("user_name",b_string{m_user->user_name()})
            ));
            p.add_fields(make_document(
                kvp("players", make_document(
                    kvp("$arrayElemAt", make_array(
                        "$players",
                        make_document(
                            kvp("$getField","current_player_id")
                        )
                    ))
                ))
            ));
            db::AggregateOperation find_one{"users",std::move(p)};
            find_one.ExecuteOperation();
            auto result = find_one.GetResult();
            if (result) 
            {
                auto doc = result.value();
                m_user->clear_players();
                auto player_doc = doc["players"].get_document().view();
                auto temp = m_user->add_players();
                temp->set_guild_id(static_cast<uint64_t>(player_doc["guild_id"].get_int64())); 
                return true;
            }
            return false;
        }
    };

    
}


