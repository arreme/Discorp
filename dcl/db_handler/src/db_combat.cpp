#include <db_handler/db_combat.hpp>

using namespace db_handler;

#pragma region private_methods

bsoncxx::document::value DBCombatHandler::SkillsToBson(const PBSkills &skills) 
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

void DBCombatHandler::BsonToSkills(PBSkills *skills, bsoncxx::document::view doc) 
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

bsoncxx::document::value DBCombatHandler::StatsToBson(const PBStats &stats) 
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

void DBCombatHandler::BsonToStats(PBStats *stats, bsoncxx::document::view doc) 
{
    stats->set_max_health(doc["max_health"].get_int32());
    stats->set_current_health(doc["current_health"].get_int32());
    stats->set_strength(doc["strength"].get_int32());
    stats->set_defense(doc["defense"].get_int32());
    stats->set_precision(doc["precision"].get_int32());
    stats->set_speed(doc["speed"].get_int32());
    stats->set_luck(doc["luck"].get_int32());
};

bsoncxx::document::value DBCombatHandler::EquipationToBson(const PBEquipation &equipation) 
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

void DBCombatHandler::BsonToEquipation(PBEquipation *equipation, bsoncxx::document::view doc) 
{
    equipation->set_armor_head(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_head"].get_int32())));
    equipation->set_armor_chest(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_chest"].get_int32())));
    equipation->set_armor_leggings(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_leggings"].get_int32())));
    equipation->set_armor_boots(static_cast<PBItemEnum>(static_cast<int32_t>(doc["armor_boots"].get_int32())));
    equipation->set_weapon(static_cast<PBItemEnum>(static_cast<int32_t>(doc["weapon"].get_int32())));
    equipation->set_tool(static_cast<PBItemEnum>(static_cast<int32_t>(doc["tool"].get_int32())));

    for(auto const& attack : doc["attacks"].get_array().value) 
    {
        equipation->add_attacks(static_cast<PBItemEnum>(static_cast<int32_t>(attack.get_int32())));
    }
}

bsoncxx::document::value DBCombatHandler::PlayerToBson(const PBPlayer &player) {
    return make_document(
        kvp("guild_id",b_int64{static_cast<int64_t>(player.guild_id())}),
        kvp("stats", StatsToBson(player.stats())),
        kvp("skills",SkillsToBson(player.skills())),
        kvp("equipation",EquipationToBson(player.equipation())),
        kvp("gold",b_int64{player.gold()})
    );
}

void DBCombatHandler::BsonToPlayer(PBPlayer *player, bsoncxx::document::view doc) {
    player->set_guild_id(static_cast<uint64_t>(doc["guild_id"].get_int64())); 
    BsonToStats(player->mutable_stats(),doc["stats"].get_document().view());
    BsonToSkills(player->mutable_skills(),doc["skills"].get_document().view());
    BsonToEquipation(player->mutable_equipation(),doc["equipation"].get_document().view());
    player->set_gold(doc["gold"].get_int64());
}

#pragma endregion

#pragma region db_methods

bool DBCombatHandler::FindCurrentCombat() 
{
    db::FindOneOperation find_one{"combat",make_document(
        kvp("starter_user_id",b_int64{static_cast<int64_t>(m_combat_db->starter_user_id())}),
        kvp("opponent_user_id",b_int64{static_cast<int64_t>(m_combat_db->opponent_user_id())})
    )};

    find_one.ExecuteOperation();

    if (find_one.GetState() == db::OperationState::SUCCESS && find_one.m_result.has_value()) 
    {
        auto const doc = find_one.m_result.value().view();
        m_combat_db->set_wager(doc["wager"].get_int32());
        m_combat_db->set_turn(doc["turn"].get_int32());
        m_combat_db->set_starter_action(static_cast<PBCombatActions>(static_cast<int32_t>(doc["starter_action"].get_int32())));
        m_combat_db->set_opponent_action(static_cast<PBCombatActions>(static_cast<int32_t>(doc["opponent_action"].get_int32())));
        BsonToPlayer(m_combat_db->mutable_starter_user_info(),doc["starter_user_info"].get_document().view());
        BsonToPlayer(m_combat_db->mutable_opponent_user_info(),doc["opponent_user_info"].get_document().view());
        return true;
    }

    return false;
};

bool DBCombatHandler::InsertNewCombat() 
{
    db::InsertOneOperation insert_one{"combat",make_document(
        kvp("starter_user_id",b_int64{static_cast<int64_t>(m_combat_db->starter_user_id())}),
        kvp("opponent_user_id",b_int64{static_cast<int64_t>(m_combat_db->opponent_user_id())}),
        kvp("wager", b_int32{m_combat_db->wager()}),
        kvp("turn", b_int32{m_combat_db->turn()}),
        kvp("starter_action", b_int32{m_combat_db->starter_action()}),
        kvp("opponent_action", b_int32{m_combat_db->opponent_action()}),
        kvp("starter_user_info",b_document{PlayerToBson(m_combat_db->starter_user_info())}),
        kvp("opponent_user_info",b_document{PlayerToBson(m_combat_db->opponent_user_info())})
    )};

    insert_one.ExecuteOperation();
    return insert_one.GetState() == db::OperationState::SUCCESS;
};

bool DBCombatHandler::UpdateCombatForPlayer(bool starting_player, bool opponent_player) 
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("turn",b_int32{m_combat_db->turn()}));
    if (starting_player) {
        doc.append(kvp("starter_user_info",b_document{PlayerToBson(m_combat_db->starter_user_info())}));
    }
    if (opponent_player) {
        doc.append(kvp("opponent_user_info",b_document{PlayerToBson(m_combat_db->opponent_user_info())}));
    }

    db::UpdateOneOperation update_one{"combat",make_document(
        kvp("starter_user_id",b_int64{static_cast<int64_t>(m_combat_db->starter_user_id())}),
        kvp("opponent_user_id",b_int64{static_cast<int64_t>(m_combat_db->opponent_user_id())})
    ), make_document(kvp("$set",b_document{doc.extract()}))};
    
    update_one.ExecuteOperation();
    return update_one.GetState() == db::OperationState::SUCCESS;
};

bool DBCombatHandler::DeleteCombat() 
{
    db::DeleteOneOperation delete_one{"combat",make_document(
        kvp("starter_user_id",b_int64{static_cast<int64_t>(m_combat_db->starter_user_id())}),
        kvp("opponent_user_id",b_int64{static_cast<int64_t>(m_combat_db->opponent_user_id())})
    )};

    delete_one.ExecuteOperation();
    return delete_one.GetState() == db::OperationState::SUCCESS;
};

#pragma endregion