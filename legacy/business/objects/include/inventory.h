#pragma once

#include <object.h>
#include <vector>

struct InventoryCell
{
    Object objectType;
    int quantity;
};


class Inventory
{
private:
    std::vector<InventoryCell> objectList;
    
public:
    Inventory();
    bool AddObject(Object item);
};
