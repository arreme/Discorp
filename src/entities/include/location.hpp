#pragma once
#include <iostream>
#include <time.h>
#include <game_constants.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <filesystem>
#include <fstream>

class Post 
{
private:
    std::string m_post_name;
    uint8_t m_capacity_lvl;
    uint8_t m_gen_second_lvl;
    uint8_t m_fortune_lvl;
    uint8_t m_auto_collect_lvl;
    std::chrono::system_clock::time_point m_last_updated;
    double m_resource_stored = 0.0f;
public:
    Post(std::string post_name,uint8_t capacity_lvl,uint8_t generation_lvl, uint8_t fortune_lvl, uint8_t auto_collect_lvl);
    Post(bsoncxx::document::view doc);
    bsoncxx::document::value ToJson() const;
};

class Location 
{
private:
    g_enums::GameLocations m_location_id = g_enums::GameLocations::ERROR_LOCATION;
    //bool m_is_unlocked = true;
    std::unordered_map<uint8_t,Post> m_location_posts;
public:
    static Location LocationBuilder(g_enums::GameLocations loc_id);
    Location(bsoncxx::document::view doc);
    //void Image();
    void MinePost(uint8_t id);

    bsoncxx::document::value ToJson() const;

    g_enums::GameLocations GetLocId() const;
};

