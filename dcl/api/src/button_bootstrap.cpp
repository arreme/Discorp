#include <api/button_bootstrap.hpp>

ButtonBootstrap::ButtonBootstrap(dpp::cluster *bot) 
{
}


Button *ButtonBootstrap::Find(std::string button_name) 
{
    return button_list.at(button_name).get();
}