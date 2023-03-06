#include <db_handler/entities/interaction.hpp>

using bsoncxx::builder::basic::kvp;

PostInfo::PostInfo(bsoncxx::document::view doc) 
{
    m_capacity_lvl = doc["capacity_lvl"].get_int32();
    m_gen_second_lvl = doc["generation_lvl"].get_int32();
    m_fortune_lvl = doc["fortune_lvl"].get_int32();
    m_auto_collect_lvl = doc["autocollect_lvl"].get_int32();
    m_last_collected = doc["last_updated"].get_date();
    m_resource_stored = doc["current_stored"].get_double();
}

bsoncxx::document::value PostInfo::ToJson() const
{
    bsoncxx::builder::basic::document doc{};

    doc.append(kvp("capacity_lvl",bsoncxx::types::b_int32{m_capacity_lvl}));
    doc.append(kvp("generation_lvl",bsoncxx::types::b_int32{m_gen_second_lvl}));
    doc.append(kvp("fortune_lvl",bsoncxx::types::b_int32{m_fortune_lvl}));
    doc.append(kvp("autocollect_lvl",bsoncxx::types::b_int32{m_auto_collect_lvl}));
    doc.append(kvp("last_updated", bsoncxx::types::b_date{m_last_collected}));
    doc.append(kvp("current_stored",bsoncxx::types::b_double{m_resource_stored}));

    return doc.extract();
}

bsoncxx::document::value ZoneAccessInfo::ToJson() const 
{
    bsoncxx::builder::basic::document doc{};

    doc.append(kvp("is_unlocked",bsoncxx::types::b_bool{m_is_unlocked}));

    return doc.extract();
}

ZoneAccessInfo::ZoneAccessInfo(bsoncxx::document::view doc) 
{
    m_is_unlocked = doc["is_unlocked"].get_bool();
}