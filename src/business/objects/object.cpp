#include <object.h>

Object::Object(std::string itemName, bool canSell, int marketValue,std::string resourceName) :
itemName(itemName),canSell(canSell),marketValue(marketValue) ,resourceName(resourceName)
{
    
};