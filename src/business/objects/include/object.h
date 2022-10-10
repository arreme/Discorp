#pragma once
#include <stdio.h>
#include <string>
#include <gd.h>
class Object
{
private:
    std::string itemName;
    bool canSell;
    int marketValue;
    std::string resourceName;
public:
    Object(std::string itemName, bool canSell, int marketValue,std::string resourceName);
};
