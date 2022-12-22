#pragma once
#include <iostream>
#include <memory>
#include <vector>

using Asset = const std::unique_ptr<FILE, decltype(std::fclose) *>;

Asset background1(std::fopen("resources/Background1.png", "rb"), std::fclose);



class Farm
{
private:
    std::string farm_id;
    std::vector<std::vector<uint8_t>> plants;
};