#pragma once
#include <core/pb/interaction.pb.h>
#include <core/game_logic.hpp>
#include <google/protobuf/util/time_util.h>

namespace DCLInteractions 
{
    class DCLInteractionType
    {
    public:
        virtual bool FillInteractionInfo(const PBInteraction *info) = 0;
        virtual PBInteractionType GetType() const = 0;
        virtual int CalculateImageIndex(const PBInteraction &interaction_db) const = 0;
    };

    class DCLEmptyInteraction : public DCLInteractionType
    {
    public:
        DCLEmptyInteraction() {}
        
        bool FillInteractionInfo(const PBInteraction *info) override 
        {
            return false;
        }

        PBInteractionType GetType() const override
        {
            return PBInteractionType::NONE;
        }

        int CalculateImageIndex(const PBInteraction &interaction_db) const override 
        {
            return 0;
        }
        
    };

    class DCLPostInteraction : public DCLInteractionType
    {
    private:
        PBPostInteraction m_post_info;
    public:
        DCLPostInteraction(PBPostInteraction *post_info) 
        {
            m_post_info.Swap(post_info);
        }

        bool FillInteractionInfo(const PBInteraction *info) override 
        {
            for (auto const &type : info->types())
            {
                if (type == PBInteractionType::POST) 
                {
                    //m_info = info;
                    return true;
                }
            }
            return false;
        }

        PBInteractionType GetType() const override
        {
            return PBInteractionType::POST;
        }

        int CalculateImageIndex(const PBInteraction &interaction_db) const override 
        {
            auto post_db = interaction_db.post_info();
            return (post_db.capacity_upgrade() + post_db.gen_second_upgrade() + post_db.fortune_upgrade()) / 3;
        }

        const google::protobuf::RepeatedPtrField<PBItemData> GetResources() const
        {
            return m_post_info.resources();
        }

        float GetCurrentStat(PBUpgradeType upgrade_type, int level) const
        {
            if (!PBUpgradeType_IsValid(upgrade_type)) return -1;
            if (level < 0 || level >= m_post_info.upgrades(upgrade_type).info().size()) return -1;
            return m_post_info.upgrades(upgrade_type).info(level).current_stat();
        }

        const google::protobuf::RepeatedPtrField<PBItemData> &GetUpgradeRequirements(PBUpgradeType type, int level) const
        {
            
            if (!PBUpgradeType_IsValid(type)) type = PBUpgradeType::CAPACITY;
            if (level < 0 || level >= m_post_info.upgrades(type).info_size()) level = 0;
            return m_post_info.upgrades(type).info(level).upgrade_req();
        }

        std::vector<PBItemData> CalculatePostRewards(PBPlayer *player_db, PBPostInteraction *post_db) const 
        {
            std::vector<PBItemData> resources_to_add;
            std::chrono::time_point<std::chrono::system_clock> dt(std::chrono::seconds(google::protobuf::util::TimeUtil::TimestampToSeconds(post_db->last_collected())));
            auto seconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - dt).count());
            int max_capacity = m_post_info.upgrades(PBUpgradeType::CAPACITY).info(post_db->capacity_upgrade()).current_stat();
            auto gen_second = m_post_info.upgrades(PBUpgradeType::GEN_SECOND).info(post_db->gen_second_upgrade()).current_stat();
            auto fortune = m_post_info.upgrades(PBUpgradeType::FORTUNE).info(post_db->fortune_upgrade()).current_stat();
            int added = std::floor(seconds / gen_second);
            if (added != 0) 
            {
                auto last_collected = post_db->mutable_last_collected();
                auto now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                last_collected->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(now));
            }
            auto total = std::min(added + post_db->resource_stored(), max_capacity);
            auto modifier = GameLogic::CalculateModifier(m_post_info.post_skill(), *player_db);
            auto gathered = std::min(total, 1 + modifier);
            post_db->set_resource_stored(std::max(total - gathered, 0));
            GameLogic::CalculateLevel(m_post_info.post_skill(), gathered, m_post_info.interact_xp(), player_db);
            auto prob = rand() % 100;
            if (prob <= (player_db->stats().luck() + fortune)) gathered += 1 + gathered*0.2f;

            for(auto const &item_data : m_post_info.resources()) 
            {
                PBItemData temp;
                temp.set_item_id(item_data.item_id());
                auto result = gathered * (item_data.quantity()/100);
                if (result >= 1) {
                    temp.set_quantity(result);
                    resources_to_add.push_back(temp);
                }   
            }
            return resources_to_add;
        }
        
    };

    class DCLZoneAccessInteraction : public DCLInteractionType 
    {
    private:
        PBZoneAccessInteraction m_zone_access_info;
    public:
        DCLZoneAccessInteraction(PBZoneAccessInteraction *zone_access_info) : m_zone_access_info(std::move(*zone_access_info)) {}

        bool FillInteractionInfo(const PBInteraction *info) override 
        {
            for (auto const &type : info->types())
            {
                if (type == PBInteractionType::ZONE_ACCESS) 
                {
                    //m_info = info;
                    return true;
                }
            }
            return false;
        }

        PBInteractionType GetType() const override
        {
            return PBInteractionType::ZONE_ACCESS;
        }

        int CalculateImageIndex(const PBInteraction &interaction_db) const override 
        {
            return interaction_db.zone_access_info().unlock_level();
        }

        const google::protobuf::RepeatedPtrField<PBItemData> &GetRequirements(int level) const 
        {
            return m_zone_access_info.unlock_info(level).upgrade_req();
        }

        bool IsUnlocked(int level) const
        {
            return m_zone_access_info.unlock_info(level).current_stat() == 1;
        }

        const google::protobuf::RepeatedPtrField<PBItemData> &GetUpgradeRequirements(int level) const
        {
            if (level < 0 || level >= m_zone_access_info.unlock_info_size()) level = 0;
            return m_zone_access_info.unlock_info(level).upgrade_req();
        }

        PBLocationID GetNextLocation() const
        {
            return m_zone_access_info.next_loc();
        }
    };

    class DCLDialogInteraction : public DCLInteractionType 
    {
    private:
        PBDialogInteraction m_dialog_info;
    public:
        DCLDialogInteraction(PBDialogInteraction *m_dialog_info) : m_dialog_info(std::move(*m_dialog_info)) {}

        bool FillInteractionInfo(const PBInteraction *info) override 
        {
            for (auto const &type : info->types())
            {
                if (type == PBInteractionType::DIALOG) 
                {
                    //m_info = info;
                    return true;
                }
            }
            return false;
        }

        PBInteractionType GetType() const override
        {
            return PBInteractionType::DIALOG;
        }

        int CalculateImageIndex(const PBInteraction &interaction_db) const override 
        {
            return 0;
        }
    };
}
