#include <api/button_bootstrap.hpp>

ButtonBootstrap::ButtonBootstrap(dpp::cluster *bot) 
{
    button_list.insert({"upgrade_post",std::make_unique<UpgradePostButton>(bot)});
    //button_list.insert({"collect_post",});
    //button_list.insert({"go_to_location",});
    //button_list.insert({"unlock_zone",});
    //button_list.insert({"interact",});
    //button_list.insert({"inventory",});
    //button_list.insert({"profile",});
}


Button *ButtonBootstrap::Find(std::string button_name) 
{
    return button_list.at(button_name).get();
}