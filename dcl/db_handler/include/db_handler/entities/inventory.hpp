#pragma once
#include <iostream>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

class Item 
{
private:
    int32_t m_item_id = 0;
    int32_t m_quantity = 0;
public:
    inline static const std::string RESOURCE_TYPE = "resources";
    inline static const std::string UTILITIES_TYPE = "utilities";
    inline static const std::string EQUIPMENT_TYPE = "equipment";

    Item(int32_t item_id, int32_t quantity);

    Item(bsoncxx::document::view bson);

    int32_t GetItemId();

    int32_t GetQuantity();
    
};