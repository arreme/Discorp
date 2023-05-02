#pragma once
#include <core/pb/map.pb.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <google/protobuf/util/json_util.h>
#include <core/interaction_wrappers.hpp>

namespace DCLData 
{
    const std::string MAIN_MAP_DATA = "resources/data/locations";
    const std::string ITEMS_DATA = "resources/data/items";

    class DCLLocation 
    {
    private:
        PBLocation m_location;
    public:
        DCLLocation(PBLocation &&location) 
        : m_location(location)
        {}

        bool GetInteraction(DCLInteractions::DCLInteraction &interaction, int id) const 
        {
            return interaction.FillInteractionInfo(&m_location.interactions(id));
        }

        PBLocation GetLocationDB() const
        {
            PBLocation location;
            std::cout << m_location.DebugString() << std::endl;
            for (const auto &interaction : m_location.interactions()) 
            {
                auto temp_interaction = location.add_interactions();
                for (const auto &type : interaction.types()) 
                {
                    switch (type)
                    {
                    case PBInteractionType::POST:
                        if (interaction.post_info().needs_database()) 
                        {
                            temp_interaction->add_types(PBInteractionType::POST);
                            std::cout << location.DebugString() << std::endl;
                        }
                        break;
                    case PBInteractionType::DIALOG:
                        if (interaction.post_info().needs_database()) 
                        {
                            temp_interaction->add_types(PBInteractionType::DIALOG);
                        }
                        break;
                    case PBInteractionType::ZONE_ACCESS:
                        if (interaction.post_info().needs_database()) 
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

        const PBLocation *const GetLocationData() const 
        {
            return &m_location;
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

        const PBLocation *const GetLocationData(PBLocationID locId) const noexcept 
        {
            try
            {
                return m_locations.at(locId).GetLocationData();
            }
            catch(const std::out_of_range& e)
            {}

            return nullptr;
        }

        DCLMap(DCLMap const&)          = delete;
        void operator=(DCLMap const&)  = delete;
    };
}
