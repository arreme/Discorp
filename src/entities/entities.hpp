#pragma once
#include <iostream>
#include <memory>

using Asset = const std::unique_ptr<FILE, decltype(std::fclose) *>;

Asset background1(std::fopen("resources/Background1.png", "rb"), std::fclose);


class User 
{
private:
    std::string id;
    std::string location;
};