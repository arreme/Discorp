#pragma once
#include <core/pb/interaction.pb.h>


namespace DCLInteractions 
{
    class DCLInteraction 
    {
    protected:
        const PBInteraction *m_info;
    public:
        virtual bool FillInteractionInfo(const PBInteraction *info) = 0;
    };

    class DCLPostInteraction : public DCLInteraction
    {

    public:
        virtual bool FillInteractionInfo(const PBInteraction *info) override 
        {
            for (auto const &type : info->types())
            {
                if (type == PBInteractionType::POST) 
                {
                    m_info = info;
                    return true;
                }
            }
            return false;
        }

        
    };

    class DCLZoneAccessInteraction : public DCLInteraction 
    {
    public:
        virtual bool FillInteractionInfo(const PBInteraction *info) override 
        {
            for (auto const &type : info->types())
            {
                if (type == PBInteractionType::ZONE_ACCESS) 
                {
                    m_info = info;
                    return true;
                }
            }
            return false;
        }
    };
}
