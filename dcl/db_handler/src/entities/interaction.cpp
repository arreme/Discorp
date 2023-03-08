#include <db_handler/entities/interaction.hpp>

using bsoncxx::builder::basic::kvp;

/***************
 *  POST INFO  *
 ***************/

PostInfo::PostInfo() 
{
    m_type = InteractionType::POST;
}

PostInfo::PostInfo(bsoncxx::document::view doc) 
{
    m_type = InteractionType::POST;
    m_capacity_lvl = doc["capacity_lvl"].get_int32();
    m_gen_second_lvl = doc["generation_lvl"].get_int32();
    m_fortune_lvl = doc["fortune_lvl"].get_int32();
    m_auto_collect_lvl = doc["autocollect_lvl"].get_int32();
    m_last_collected = doc["last_updated"].get_date();
}

bsoncxx::document::value PostInfo::ToJson() const
{
    bsoncxx::builder::basic::document doc{};

    doc.append(kvp("type", bsoncxx::types::b_int32{static_cast<int>(m_type)}));
    doc.append(kvp("capacity_lvl",bsoncxx::types::b_int32{m_capacity_lvl}));
    doc.append(kvp("generation_lvl",bsoncxx::types::b_int32{m_gen_second_lvl}));
    doc.append(kvp("fortune_lvl",bsoncxx::types::b_int32{m_fortune_lvl}));
    doc.append(kvp("autocollect_lvl",bsoncxx::types::b_int32{m_auto_collect_lvl}));
    doc.append(kvp("last_updated", bsoncxx::types::b_date{m_last_collected}));

    return doc.extract();
}

/**********************
 *  ZONE ACCESS INFO  *
 **********************/

ZoneAccessInfo::ZoneAccessInfo() 
{
    m_type = InteractionType::ZONE_ACCESS;
}

ZoneAccessInfo::ZoneAccessInfo(bsoncxx::document::view doc) 
{
    m_type = InteractionType::ZONE_ACCESS;
    m_is_unlocked = doc["is_unlocked"].get_bool();
}

bsoncxx::document::value ZoneAccessInfo::ToJson() const 
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("type", bsoncxx::types::b_int32{static_cast<int>(m_type)}));
    doc.append(kvp("is_unlocked",bsoncxx::types::b_bool{m_is_unlocked}));
    return doc.extract();
}

/**********************
 *  INTERACTION INFO  *
 **********************/

InteractionType InteractionInfo::GetType() 
{
    return m_type;
}

std::vector<std::unique_ptr<InteractionInfo>> InteractionInfo::CreateInteractions(bsoncxx::document::view doc) 
{
    std::vector<std::unique_ptr<InteractionInfo>> loc{};
    for (auto element : doc["interactions"].get_array().value)
    {
        auto doc_element = element.get_document().view();
        switch (static_cast<InteractionType>(doc_element["type"].get_int32().value)) 
        {
            case InteractionType::POST:
                loc.push_back(std::unique_ptr<InteractionInfo>(new PostInfo{doc_element}));
                break;
            case InteractionType::ZONE_ACCESS:
                loc.push_back(std::unique_ptr<InteractionInfo>(new ZoneAccessInfo{doc_element}));
                break;
            default:
                break;
        }
    }

    return loc;
    
}

std::unique_ptr<InteractionInfo> InteractionInfo::CreateInteraction(bsoncxx::document::view doc) 
{
    std::unique_ptr<InteractionInfo> loc{};
    auto interaction = doc["interactions"].get_document().view();
    switch (static_cast<InteractionType>(interaction["type"].get_int32().value)) 
    {
        case InteractionType::POST:
            return std::unique_ptr<InteractionInfo>(new PostInfo{interaction});
            break;
        case InteractionType::ZONE_ACCESS:
            return std::unique_ptr<InteractionInfo>(new ZoneAccessInfo{interaction});
            break;
        default:
            break;
    }

    return nullptr;
    
}