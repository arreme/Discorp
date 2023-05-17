#pragma once
#include <core/pb/interaction.pb.h>


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
