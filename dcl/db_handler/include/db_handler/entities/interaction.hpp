#pragma once
#include <iostream>
#include <time.h>
#include <util/game_constants.hpp>
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
    InteractionType m_type = InteractionType::NOT_DEFINED;
public:
    InteractionType GetType();
    virtual bsoncxx::document::value ToJson() const = 0;
    static std::vector<std::unique_ptr<InteractionInfo>> CreateInteractions(bsoncxx::document::view doc);
    static std::unique_ptr<InteractionInfo> CreateInteraction(bsoncxx::document::view doc);
};

class PostInfo : public InteractionInfo
{
private:
    int32_t m_capacity_lvl = 0;
    int32_t m_gen_second_lvl = 0;
    int32_t m_fortune_lvl = 0;
    std::chrono::system_clock::time_point m_last_collected = std::chrono::system_clock::now();
public:
    PostInfo();
    PostInfo(bsoncxx::document::view doc);
    bsoncxx::document::value ToJson() const override;

    int32_t GetCapacityLvl();
    int32_t GetGenSecondLvl();
    int32_t GetFortuneLvl();
};

class ZoneAccessInfo : public InteractionInfo 
{
private:
    bool m_is_unlocked = false;
public:
    ZoneAccessInfo();
    ZoneAccessInfo(bsoncxx::document::view doc);
    bsoncxx::document::value ToJson() const override;
    bool IsUnlocked();
};

