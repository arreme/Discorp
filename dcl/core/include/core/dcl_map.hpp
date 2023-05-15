#pragma once
#include <core/pb/map.pb.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <google/protobuf/util/json_util.h>
#include <core/interaction_wrappers.hpp>
#include <google/protobuf/util/time_util.h>

namespace DCLData 
{
    const std::string MAIN_MAP_DATA = "resources/data/locations";

    class DCLInteraction 
    {
    private:
        PBInteraction m_interaction;
        std::vector<std::unique_ptr<DCLInteractions::DCLInteractionType>> m_types;
    public:
        DCLInteraction(PBInteraction &&interaction) 
        : m_interaction(std::move(interaction))
        {
            std::cout << m_interaction.DebugString() << std::endl;
            for (auto const &type : m_interaction.types())
            {
                std::cout << type << std::endl;
                switch (type)
                {
                case PBInteractionType::POST:
                    std::cout << "WHAT" << std::endl;
                    std::cout << m_interaction.post_info().DebugString() << std::endl;
                    m_types.push_back(std::make_unique<DCLInteractions::DCLPostInteraction>(m_interaction.mutable_post_info()));
                    break;
                case PBInteractionType::ZONE_ACCESS:
                    m_types.push_back(std::make_unique<DCLInteractions::DCLZoneAccessInteraction>(m_interaction.mutable_zone_access_info()));
                    break;
                case PBInteractionType::DIALOG:
                    m_types.push_back(std::make_unique<DCLInteractions::DCLDialogInteraction>(m_interaction.mutable_dialog_info()));
                    break;
                default:
                    std::cout << "Invalid interaction detected!" << std::endl;
                    m_types.push_back(std::make_unique<DCLInteractions::DCLEmptyInteraction>());
                    break;
                }
            }
        }

        size_t GetTypesSize() const
        {
            return m_types.size();
        }

        std::vector<std::unique_ptr<DCLInteractions::DCLInteractionType>>::const_iterator begin() const
        {
            return m_types.begin();
        }

        std::vector<std::unique_ptr<DCLInteractions::DCLInteractionType>>::const_iterator end() const
        {
            return m_types.end();
        }

        const DCLInteractions::DCLInteractionType *GetMainType() const
        {
            return m_types.at(0).get();
        }

        const DCLInteractions::DCLInteractionType *GetType(int id) const 
        {
            if (id < 0 || id >= m_interaction.types().size()) return nullptr;
            return m_types.at(id).get();
        }

        const DCLInteractions::DCLPostInteraction *TryGetPost() const
        {
            for(auto const &type : m_types) 
            {
                std::cout << type->GetType() << std::endl;
                if (type->GetType() == PBInteractionType::POST) 
                {
                    std::cout << "POST FOUND" << std::endl;
                    return static_cast<DCLInteractions::DCLPostInteraction *>(type.get());
                }
            }
            return nullptr;
        }

        const DCLInteractions::DCLZoneAccessInteraction *TryGetZoneAccess() const 
        {
            for(auto const &type : m_types) 
            {
                if (type->GetType() == PBInteractionType::ZONE_ACCESS) 
                {
                    return static_cast<DCLInteractions::DCLZoneAccessInteraction *>(type.get());
                }
            }
            return nullptr;
        }

        const DCLInteractions::DCLDialogInteraction *TryGetDialog() const 
        {
            for(auto const &type : m_types) 
            {
                if (type->GetType() == PBInteractionType::DIALOG) 
                {
                    return static_cast<DCLInteractions::DCLDialogInteraction *>(type.get());
                }
            }
            return nullptr;
        }

        const std::string &GetInteractionName() const 
        {
            return m_interaction.interaction_name();
        }

        int GetDatabaseId() const 
        {
            return m_interaction.database_id();
        }

        int GetPosX() const 
        {
            return m_interaction.pos_x();
        }

        int GetPosY() const 
        {
            return m_interaction.pos_y();
        }

        const std::string &GetIconMapPath(int id) const 
        {
            return m_interaction.map_icon_paths(std::min(id, m_interaction.map_icon_paths().size() - 1));
        }

        const std::string &GetDescription() const 
        {
            return m_interaction.description();
        }

        const std::string &GetListName() const 
        {
            return m_interaction.interaction_list_name();
        }
    };

    class DCLLocation 
    {
    private:
        PBLocation m_location;
        std::vector<DCLInteraction> m_interactions;
    public:
        DCLLocation(PBLocation &&location) 
        : m_location(std::move(location))
        {
            for (auto interaction : *m_location.mutable_interactions())
            {
                m_interactions.push_back(std::move(interaction));
            }
        }

        size_t GetInteractionsLength() 
        {
            return m_interactions.size();
        }

        std::vector<DCLInteraction>::const_iterator begin() const
        {
            return m_interactions.begin();
        }

        std::vector<DCLInteraction>::const_iterator end() const
        {
            return m_interactions.end();
        }

        const DCLInteraction *GetInteraction(int id) const
        {
            if (id < 0 || id >= m_interactions.size()) return nullptr;
            return &m_interactions.at(id);
        }

        PBLocation GetLocationDB() const
        {
            PBLocation location;
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

        bool NeedsDatabase() const 
        {
            return m_location.database_id() != -1;
        };

        const std::string &GetLocationName() const 
        {
            return m_location.name();
        }

        const std::string &GetLocationImage() const 
        {
            return m_location.image();
        }
    };

    class DCLMap 
    {
    private:
        std::map<PBLocationID,const DCLLocation> m_locations;
        DCLMap() 
        {
            for (const auto &entry : std::filesystem::directory_iterator(MAIN_MAP_DATA)) 
            {
                std::ifstream t(entry.path());
                std::stringstream buffer;
                buffer << t.rdbuf();
                auto loc_data = PBLocation{};
                google::protobuf::util::JsonStringToMessage(buffer.str(),&loc_data);
                PBLocationID loc_id = loc_data.id();
                std::cout << loc_data.id() << std::endl;
                std::cout << entry.path() << std::endl;
                m_locations.emplace(loc_id,std::move(loc_data));
            }
        } 
    public:
        static DCLMap& getInstance()
        {
            static DCLMap instance;
            return instance;
        }

        const DCLLocation* const GetLocation(PBLocationID locId) const noexcept 
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
