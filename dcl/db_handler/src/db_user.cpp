#include <db_handler/db_user.hpp>

namespace db_handler 
{

#pragma region private_methods

    bsoncxx::document::value DBUserHandler::SkillsToBson(const PBSkills &skills) 
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

    void DBUserHandler::BsonToSkills(PBSkills *skills, bsoncxx::document::view doc) 
    {
        skills->set_foraging_lvl(doc["foraging_lvl"].get_int32());
        skills->set_foraging_xp(doc["foraging_xp"].get_int64());
        skills->set_mining_lvl(doc["mining_lvl"].get_int32());
        skills->set_mining_xp(doc["mining_xp"].get_int64());
        skills->set_combat_lvl(doc["combat_lvl"].get_int32());
        skills->set_combat_xp(doc["combat_xp"].get_int64());
        skills->set_athletics_lvl(doc["athletics_lvl"].get_int32());
        skills->set_athletics_xp(doc["athletics_xp"].get_int64());
    }

    bsoncxx::document::value DBUserHandler::StatsToBson(const PBStats &stats) 
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

    void DBUserHandler::BsonToStats(PBStats *stats, bsoncxx::document::view doc) 
    {
        stats->set_max_health(doc["max_health"].get_int32());
        stats->set_current_health(doc["current_health"].get_int32());
        stats->set_strength(doc["strength"].get_int32());
        stats->set_defense(doc["defense"].get_int32());
        stats->set_precision(doc["precision"].get_int32());
        stats->set_speed(doc["speed"].get_int32());
        stats->set_luck(doc["luck"].get_int32());
    };

    bsoncxx::document::value DBUserHandler::EquipationToBson(const PBEquipation &equipation) 
    {
        bsoncxx::builder::basic::document doc{};
        doc.append(kvp("armor_head",b_int32{static_cast<int32_t>(equipation.armor_head())}));
        doc.append(kvp("armor_chest",b_int32{static_cast<int32_t>(equipation.armor_chest())}));
        doc.append(kvp("armor_leggings",b_int32{static_cast<int32_t>(equipation.armor_leggings())}));
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

    void DBUserHandler::BsonToEquipation(PBEquipation *equipation, bsoncxx::document::view doc) 
    {
        equipation->set_armor_head(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_head"].get_int32())));
        equipation->set_armor_chest(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_chest"].get_int32())));
        equipation->set_armor_leggings(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_leggings"].get_int32())));
        equipation->set_armor_boots(static_cast<PBItemEnum>(static_cast<int32_t>(doc["darmor_boots"].get_int32())));
        equipation->set_weapon(static_cast<PBItemEnum>(static_cast<int32_t>(doc["weapon"].get_int32())));
        equipation->set_tool(static_cast<PBItemEnum>(static_cast<int32_t>(doc["tool"].get_int32())));

        for(auto const& attack : doc["attacks"].get_array().value) 
        {
            equipation->add_attacks(static_cast<PBItemEnum>(static_cast<int32_t>(attack.get_int32())));
        }
    }

    bsoncxx::document::value DBUserHandler::PlayerToBson(const PBPlayer &player) {
        return make_document(
            kvp("guild_id",b_int64{static_cast<int64>(player.guild_id())}),
            kvp("current_location",b_int32{static_cast<int32>(player.current_location())}),
            kvp("stats", StatsToBson(player.stats())),
            kvp("skills",SkillsToBson(player.skills())),
            kvp("equipation",EquipationToBson(player.equipation())),
            kvp("gold",b_int64{player.gold()})
        );
    }

    void DBUserHandler::BsonToPlayer(PBPlayer *player, bsoncxx::document::view doc) {
        player->set_guild_id(static_cast<uint64_t>(doc["guild_id"].get_int64())); 
        player->set_current_location(static_cast<PBLocationID>(static_cast<int32_t>(doc["current_location"].get_int32())));
        BsonToStats(player->mutable_stats(),doc["stats"].get_document().view());
        BsonToSkills(player->mutable_skills(),doc["skills"].get_document().view());
        BsonToEquipation(player->mutable_equipation(),doc["equipation"].get_document().view());
        player->set_gold(doc["gold"].get_int64());
    }

#pragma endregion

#pragma region public_methods

    DBUserHandler::DBUserHandler(PBUser *user)
    : m_user(user)  
    {};

    bool DBUserHandler::InsertUser(db::Transaction *t) noexcept
    {
        if (!m_user) return false;
        bsoncxx::builder::basic::document doc{};
        doc.append(kvp("discord_id",b_int64{static_cast<int64>(m_user->discord_id())}));
        doc.append(kvp("user_name",b_string{m_user->user_name()}));
        doc.append(kvp("current_player_id",b_int32{m_user->current_player_id()}));
        doc.append(kvp("last_online",b_date{std::chrono::seconds(TimeUtil::TimestampToSeconds(m_user->last_online()))}));

        bsoncxx::builder::basic::array players{};
        for (auto const& player : m_user->players())
        {
            players.append(PlayerToBson(player));
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

    bool DBUserHandler::FindUserCurrentPlayer() noexcept
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
        auto result = find_one.GetFirstResult();
        if (result) 
        {
            m_user->Clear();
            auto doc = result.value();
            try {
                m_user->set_discord_id(static_cast<uint64_t>(doc["discord_id"].get_int64()));
                m_user->set_user_name(bsoncxx::string::to_string(doc["version"].get_string().value));
                m_user->set_current_player_id(static_cast<int32_t>(doc["discord_id"].get_int64()));
                m_user->mutable_last_online()->set_seconds(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
                
                auto temp = m_user->add_players();
                auto player_doc = doc["players"].get_document().view();
                BsonToPlayer(temp,player_doc);
                
            } catch (bsoncxx::exception e) {
                return false;
            }
            return true;
        }
        return false;
    }

#pragma endregion    

}