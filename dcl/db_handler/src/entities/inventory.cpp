#include <db_handler/entities/inventory.hpp>

Item::Item(int32_t item_id, int32_t quantity) 
: m_item_id(item_id), m_quantity(quantity)
{

}

Item::Item(bsoncxx::document::view bson) 
{
    m_item_id = bson["item_id"].get_int32();
    m_quantity = bson["quantity"].get_int32();
}

int32_t Item::GetItemId() 
{
    return m_item_id;
}

int32_t Item::GetQuantity() 
{
    return m_quantity;
}