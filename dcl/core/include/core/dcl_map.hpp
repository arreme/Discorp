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

        const PBLocation *GetLocation() const
        {
            return &m_location;
        }

        bool NeedsDatabase() const 
        {
            return m_location.database_id() != -1;
        };
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
                PBLocationID loc_id = loc_data.loc_id();
                std::cout << loc_data.loc_id() << std::endl;
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
