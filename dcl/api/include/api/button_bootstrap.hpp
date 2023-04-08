#pragma once
#include <vector>
#include <dpp.h>
#include <api/button.hpp>

class ButtonBootstrap 
{
private:
    std::map<std::string,std::unique_ptr<Button>> button_list;
public:
    ButtonBootstrap(dpp::cluster *bot);
    Button *Find(std::string button_name);
};