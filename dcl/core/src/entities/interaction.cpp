#include <entities/location.hpp>

using bsoncxx::builder::basic::kvp;

Location::Location(bsoncxx::document::view doc) 
{
    m_location_id = static_cast<g_enums::GameLocations>(doc["location_id"].get_int32().value);
    
    auto is_array = doc["interactions"];
    if (is_array) 
    {
        auto loc_array = is_array.get_array();
        for (auto member : loc_array.value)
        {
            auto curr = member.get_document().view();
            auto type = static_cast<InteractionType>(curr["type"].get_int32().value);
            auto id = static_cast<uint32_t>(curr["id"].get_int32());

            switch (type)
            {
            case InteractionType::POST:
                m_interactions.push_back(std::unique_ptr<Interaction>(new Post(member.get_document().view())));
                break;
            case InteractionType::ZONE_ACCESS:
                m_interactions.push_back(std::unique_ptr<Interaction>(new ZoneAccess(member.get_document().view())));
                break;
            default:
                break;
            }
            
        }
        
    }
    
}

bsoncxx::document::value Location::ToJson() const
{
    bsoncxx::builder::basic::document doc{};

    doc.append(kvp("location_id",static_cast<int>(m_location_id)));
    //doc.append(kvp("is_unlocked",bsoncxx::types::b_bool(m_is_unlocked)));
    auto loc_maps = &m_interactions;
    doc.append(kvp("posts",[loc_maps](bsoncxx::builder::basic::sub_array sub) 
    {
        for (const auto& loc : *loc_maps)
        {
            sub.append(loc->ToJson());
        }
        
    }));
    return doc.extract();
}

bsoncxx::document::value ZoneAccess::ToJson() const 
{
    bsoncxx::builder::basic::document doc{};

    return doc.extract();
}

ZoneAccess::ZoneAccess(bsoncxx::document::view doc) 
{

}

Post::Post(bsoncxx::document::view doc) 
{
    m_capacity_lvl = doc["capacity_lvl"].get_int32();
    m_gen_second_lvl = doc["generation_lvl"].get_int32();
    m_fortune_lvl = doc["fortune_lvl"].get_int32();
    m_auto_collect_lvl = doc["autocollect_lvl"].get_int32();
    m_last_collected = doc["last_updated"].get_date();
    m_resource_stored = doc["current_stored"].get_double();
}

bsoncxx::document::value Post::ToJson() const
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


Location Location::LocationBuilder(g_enums::GameLocations id) 
{
    auto res = utils::LoadLocationInfo(id);
    if(res.view().length() == 0) throw std::exception();
    return Location{res.view()};
}

g_enums::GameLocations Location::GetLocId() const 
{
    return m_location_id;
}
