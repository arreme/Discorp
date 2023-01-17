#include <location.hpp>

using bsoncxx::builder::basic::kvp;

Location::Location(g_enums::GameLocations location_id, std::string location_name, std::unordered_map<uint8_t,Post> &&locations_post) 
: m_location_id(location_id), m_location_name(location_name), m_location_posts(locations_post)
{}

Location::Location(bsoncxx::document::view doc) 
{
    m_location_id = static_cast<g_enums::GameLocations>(doc["location_id"].get_int32().value);
    m_location_name = doc["location_name"].get_utf8().value.to_string();
    auto loc_array = doc["locations"].get_array();
    uint8_t postId = 0;
    for (auto member : loc_array.value)
    {
        m_location_posts.emplace(postId,Post(member.get_document().view()));
        postId++;
    }

    
}

void Location::MinePost(uint8_t id) 
{

}

bsoncxx::document::value Location::ToJson() 
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("location_id",m_location_id));
    doc.append(kvp("location_name",m_location_name));
    auto loc_maps = m_location_posts;
    doc.append(kvp("locations",[loc_maps](bsoncxx::builder::basic::sub_array sub) 
    {
        for (auto loc : loc_maps)
        {
            sub.append(loc.second.ToJson());
        }
        
    }));
    return doc.extract();
}

Post::Post(std::string post_name,uint8_t capacity_lvl,uint8_t generation_lvl, uint8_t fortune_lvl, uint8_t auto_collect_lvl) 
: m_post_name(post_name), m_capacity_lvl(capacity_lvl), m_gen_second_lvl(generation_lvl), m_fortune_lvl(fortune_lvl), m_auto_collect_lvl(auto_collect_lvl)
{}

Post::Post(bsoncxx::document::view doc) 
{
    m_post_name = doc["post_name"].get_utf8().value.to_string();
    m_capacity_lvl = doc["capacity_lvl"].get_int32();
    m_gen_second_lvl = doc["generation_lvl"].get_int32();
    m_fortune_lvl = doc["fortune_lvl"].get_int32();
    m_auto_collect_lvl = doc["autocollect_lvl"].get_int32();
}

bsoncxx::document::value Post::ToJson() 
{
    bsoncxx::builder::basic::document doc{};
    doc.append(kvp("post_name",m_post_name));
    doc.append(kvp("capacity_lvl",m_capacity_lvl));
    doc.append(kvp("generation_lvl",m_gen_second_lvl));
    doc.append(kvp("fortune_lvl",m_fortune_lvl));
    doc.append(kvp("autocollect_lvl",m_auto_collect_lvl));
    return doc.extract();
}
