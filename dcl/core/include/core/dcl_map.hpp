#pragma once
#include <core/pb/map.pb.h>
#include <db_handler/entities/interaction.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <google/protobuf/util/json_util.h>


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
        std::vector<std::reference_wrapper<InteractionInfo>> LocationBuilder() 
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
        : m_loc_data(loc_data) 
        {}

        PBLocationID GetLocId() const 
        {
            return m_loc_data.locid();
        }
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
                m_locations.emplace(loc_data.locid(),DCLLocation{std::move(loc_data)});
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

        DCLMap(DCLMap const&)               = delete;
        void operator=(DCLMap const&)  = delete;
    };




}


