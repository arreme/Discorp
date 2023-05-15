#pragma once
#include <core/pb/map.pb.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <google/protobuf/util/json_util.h>

namespace DCLData 
{
    const std::string ITEMS_DATA = "resources/data/items";
    
    class DCLItems 
    {
    private:
        inline static std::map<PBItemType,PBItemsDict> m_item_data;
        DCLItems() 
        {
            for (const auto &entry : std::filesystem::directory_iterator(ITEMS_DATA)) 
            {
                std::ifstream t(entry.path());
                std::stringstream buffer;
                buffer << t.rdbuf();
                auto item_path = PBItemsDict{};
                google::protobuf::util::JsonStringToMessage(buffer.str(),&item_path);
                std::cout << entry.path() << std::endl;
                m_item_data.emplace(item_path.item_type(),item_path);
            }
        } 
    public:
        static const DCLItems& getInstance()
        {
            static DCLItems instance;
            return instance;
        }

        const std::string *GetItemPath(int32_t itemID) const
        {
            if (!PBItemEnum_IsValid(itemID)) return nullptr;
            int type = itemID / 1000;
            int id = itemID % 1000;
            return &m_item_data.at(static_cast<PBItemType>(type)).data(id).image_path();
        }

        //Caution can get segmentation fault
        const std::string *GetItemName(int32_t itemID) const
        {
            if (!PBItemEnum_IsValid(itemID)) return nullptr;
            
            int type = itemID / 1000;
            int id = itemID % 1000;
            return &m_item_data.at(static_cast<PBItemType>(type)).data(id).item_name();
        }

        DCLItems(DCLItems const&)        = delete;
        void operator=(DCLItems const&)  = delete;
    };
}
