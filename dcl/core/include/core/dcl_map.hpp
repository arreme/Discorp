#pragma once
#include <core/pb/map.pb.h>
#include <db_handler/entities/interaction.hpp>
#include <db_handler/entities/inventory.hpp>
#include <db_handler/entities/player.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <google/protobuf/util/json_util.h>
#include <math.h>

namespace GameLogic 
{
    bool CheckLevel(int xp, int lvl);
    int CalculateModifier(PBPlayerSkills id, Stats *const plyr_stats, Skills *const plyr_skills);
    void CalculateLevel(PBPlayerSkills id, int gathered, int resourceXp, Skills *const plyr_skills);
}

namespace GameMap 
{
    const std::string map_data_location = "resources/data";

    struct InteractionTemplates 
    {
        static PostInfo post;
        static ZoneAccessInfo zone;
    };

    class DCLLocation 
    {
    private:
        PBLocation m_loc_data;
    public:
        std::vector<std::reference_wrapper<InteractionInfo>> LocationBuilder() const
        {
            std::vector<std::reference_wrapper<InteractionInfo>> vector;
            for (auto const& curr_int : m_loc_data.interactions())
            {
                if (curr_int.databaseid() == -1) continue;

                auto type = curr_int.type();
                switch (type)
                {
                    case PBInteractionType::POST:
                        vector.push_back(InteractionTemplates::post);
                        break;
                    case PBInteractionType::ZONE_ACCESS:
                        vector.push_back(InteractionTemplates::zone);
                        break;
                    default:
                        break;
                }
            }

            return vector;
            
        }

        DCLLocation(PBLocation&& loc_data)
        {
            loc_data.Swap(&m_loc_data);          
        }

        PBLocationID GetLocId() const 
        {
            return m_loc_data.locid();
        }

        int32_t GetDatabaseID() const 
        {
            return m_loc_data.databaseid();
        }

        std::optional<int32_t> GetInteractionDatabaseID(int id) const
        {
            
            if (id >= m_loc_data.interactions().size())
            {
                return std::nullopt;
            }
            
            return m_loc_data.interactions(id).databaseid();
        }

        std::optional<PBInteractionType> GetInteractionType(int id) const
        {
            if (id >= m_loc_data.interactions().size())
            {
                return std::nullopt;
            }
            
            return m_loc_data.interactions(id).type();
        }

        std::optional<PBLocationID> GetZoneAccessNextLoc(int id) const
        {
            auto interaction = m_loc_data.interactions(id);
            if (interaction.type() != PBInteractionType::ZONE_ACCESS) {return std::nullopt;}
            return interaction.nextloc();
        }

        std::vector<Item> CalculatePostRewards(int32_t id, InteractionInfo *post_db_info,Stats *const plyr_stats,Skills *const plyr_skills) const 
        {
            std::vector<Item> resources_to_add;
            PostInfo *post_info = static_cast<PostInfo *>(post_db_info);
            auto interaction = m_loc_data.interactions(id);
            if (interaction.type() != PBInteractionType::POST) {return {};}
            auto seconds = static_cast<int>(post_info->GetDifferenceInSeconds());
            auto max_capacity = interaction.upgradeinfo(PBUpgradeType::CAPACITY).info(post_info->GetCapacityLvl()).currentstat();
            auto gen_second = interaction.upgradeinfo(PBUpgradeType::GEN_SECOND).info(post_info->GetGenSecondLvl()).currentstat();
            auto total = std::min((seconds * gen_second) + post_info->GetResourceStored(), max_capacity);
            int modifier = GameLogic::CalculateModifier(interaction.postskill(),plyr_stats,plyr_skills);
            
            auto gathered = std::min(total, 1 + modifier);
            post_info->SetResourceStored(std::max(total - gathered,0));
            GameLogic::CalculateLevel(interaction.postskill(), gathered, interaction.interactxp(), plyr_skills);
            auto prob = rand() % 100;
            if (prob <= plyr_stats->m_luck + post_info->GetFortuneLvl()) gathered += 1 + gathered*0.2f;
            resources_to_add.push_back(Item{interaction.resource(),gathered});
            return resources_to_add;
        }

        std::optional<bool> GetZoneAccessUnlocked(int id,int build_level = 0) const
        {
            auto interaction = m_loc_data.interactions(id);
            if (interaction.type() != PBInteractionType::ZONE_ACCESS) {return std::nullopt;}
            return interaction.upgradeinfo(0).info(build_level).currentstat() == 1;
        }

        std::vector<PBItemData> GetZoneAccessLevelRequirements(int32_t id, int build_level) const
        {
            auto interaction = m_loc_data.interactions(id);
            if (interaction.type() != PBInteractionType::ZONE_ACCESS) {return {};}
            auto repeated_field = interaction.upgradeinfo(0).info(build_level).uprequirements();
            
            return {repeated_field.begin(),repeated_field.end()};
        };
    };

    class DCLMap 
    {
    private:
        static std::map<PBLocationID, DCLLocation> m_locations;        

        DCLMap() 
        {
            for (const auto & entry : std::filesystem::directory_iterator(map_data_location)) 
            {
                std::ifstream t(entry.path());
                std::stringstream buffer;
                buffer << t.rdbuf();
                auto loc_data = PBLocation{};
                google::protobuf::util::JsonStringToMessage(buffer.str(),&loc_data);
                PBLocationID loc_id = loc_data.locid();
                std::cout << "LocID: " + std::to_string(loc_id) << std::endl;
                std::cout << entry.path() << std::endl;
                m_locations.emplace(loc_id,DCLLocation{std::move(loc_data)});
            }
        }

    public:
        static DCLMap& getInstance()
        {
            static DCLMap instance;
            return instance;
        }

        const DCLLocation* const GetLocation(PBLocationID locId) 
        {
            try
            {
                return &m_locations.at(locId);
            }
            catch(const std::out_of_range& e)
            {}

            return nullptr;
        }

        DCLMap(DCLMap const&)          = delete;
        void operator=(DCLMap const&)  = delete;
    };

}




