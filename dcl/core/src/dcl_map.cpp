#include <core/dcl_map.hpp>

PBLocation DCLData::DCLLocation::GetLocationDB() const
{
    PBLocation location;
    location.set_id(m_location.id());
    location.set_database_id(m_location.database_id());
    for (const auto &interaction : m_location.interactions()) 
    {
        if (interaction.database_id() == -1) continue;
        auto temp_interaction = location.add_interactions();
        for (const auto &type : interaction.types()) 
        {
            switch (type)
            {
            case PBInteractionType::POST:
                if (interaction.post_info().needs_database()) 
                {
                    temp_interaction->add_types(PBInteractionType::POST);
                    auto last_online = temp_interaction->mutable_post_info()->mutable_last_collected();
                    auto now = std::chrono::system_clock::now();
                    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
                    last_online->CopyFrom(google::protobuf::util::TimeUtil::SecondsToTimestamp(seconds));
                }
                break;
            case PBInteractionType::DIALOG:
                if (interaction.dialog_info().needs_database()) 
                {
                    temp_interaction->add_types(PBInteractionType::DIALOG);
                }
                break;
            case PBInteractionType::ZONE_ACCESS:
                if (interaction.zone_access_info().needs_database()) 
                {
                    temp_interaction->add_types(PBInteractionType::ZONE_ACCESS);
                }
                break;
            
            default:
                throw std::runtime_error{"Bad Interaction Type"};
            }
        }
    }
    
    return location;
}