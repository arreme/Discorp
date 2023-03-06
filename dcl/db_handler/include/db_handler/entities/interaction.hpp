#pragma once
#include <iostream>
#include <time.h>
#include <db_handler/game_constants.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

enum class InteractionType
{
    NOT_DEFINED = -1,
    POST = 0,
    ZONE_ACCESS = 1
};

class InteractionInfo 
{
protected:
    uint32_t m_id;
    InteractionType m_type = InteractionType::NOT_DEFINED;
public:
    virtual bsoncxx::document::value ToJson() const = 0;
};

class PostInfo : public InteractionInfo
{
private:
    int32_t m_capacity_lvl = 0;
    int32_t m_gen_second_lvl = 0;
    int32_t m_fortune_lvl = 0;
    int32_t m_auto_collect_lvl = 0;
    std::chrono::system_clock::time_point m_last_collected = std::chrono::system_clock::now();;
    double m_resource_stored = 0.0f;
public:
    PostInfo(bsoncxx::document::view doc);
    bsoncxx::document::value ToJson() const override;
};

class ZoneAccessInfo : public InteractionInfo 
{
private:
    bool m_is_unlocked = false;
public:
    ZoneAccessInfo(bsoncxx::document::view doc);
    bsoncxx::document::value ToJson() const override;
};

