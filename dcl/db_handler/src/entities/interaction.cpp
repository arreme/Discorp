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
    m_capacity_lvl = doc["CAPACITY"].get_int32();
    m_gen_second_lvl = doc["GEN_SECOND"].get_int32();
    m_fortune_lvl = doc["FORTUNE"].get_int32();
    m_resource_stored = doc["resource_stored"].get_int32();
    m_last_collected = doc["last_updated"].get_date();
}

bsoncxx::document::value PostInfo::ToJson() const
{
    bsoncxx::builder::basic::document doc{};

    doc.append(kvp("type", bsoncxx::types::b_int32{static_cast<int>(m_type)}));
    doc.append(kvp("CAPACITY",bsoncxx::types::b_int32{m_capacity_lvl}));
    doc.append(kvp("GEN_SECOND",bsoncxx::types::b_int32{m_gen_second_lvl}));
    doc.append(kvp("FORTUNE",bsoncxx::types::b_int32{m_fortune_lvl}));
    doc.append(kvp("resource_stored",bsoncxx::types::b_int32{m_resource_stored}));
    doc.append(kvp("last_updated", bsoncxx::types::b_date{m_last_collected}));

    return doc.extract();
}

int32_t PostInfo::GetCapacityLvl() 
{
    return m_capacity_lvl;
}

int32_t PostInfo::GetGenSecondLvl()
{
    return m_gen_second_lvl;
}

int32_t PostInfo::GetFortuneLvl()
{
    return m_fortune_lvl;
}

int32_t PostInfo::GetResourceStored() 
{
    m_last_collected = std::chrono::system_clock::now();
    return m_resource_stored;
}

void PostInfo::SetResourceStored(int32_t resource_stored) 
{
    m_resource_stored = resource_stored;
}

double PostInfo::GetDifferenceInSeconds() 
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - m_last_collected).count();
}

void PostInfo::ModifyLastCollected(int32_t seconds) 
{
    m_last_collected += std::chrono::seconds(seconds);
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
    m_unlocked_lvl = doc["ZONE_UNLOCK"].get_int32();
}

bsoncxx::document::value ZoneAccessInfo::ToJson() const 
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("type", bsoncxx::types::b_int32{static_cast<int>(m_type)}));
    doc.append(kvp("ZONE_UNLOCK",bsoncxx::types::b_int32{m_unlocked_lvl}));
    return doc.extract();
}

int32_t ZoneAccessInfo::GetUnlockedLvl() 
{
    return m_unlocked_lvl;
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
    if (!doc["interactions"]) return nullptr;
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