#include <api/bootstraps.hpp>

CommandBootstrap::CommandBootstrap(dpp::cluster *bot) 
{
    this->bot = bot;
    command_list.insert({"start",new StartCommand(bot->me.id)});
}

void CommandBootstrap::RegisterCommands() 
{
    std::cout << "Registering commands!" << std::endl;
    for(std::map<std::string, Command *>::iterator it = command_list.begin(); it != command_list.end(); ++it) 
    {
        Command* current = it->second;
        if (current->isGlobal) 
        {
            bot->global_command_create(current->command);
        }
    }
}

Command *CommandBootstrap::Find(std::string command_name) 
{
    return command_list.at(command_name);
}

ButtonBootstrap::ButtonBootstrap(dpp::cluster *bot) 
{
    button_list.insert({"upgrade_post",std::make_unique<UpgradePostButton>(bot)});
    button_list.insert({"cancel_button",std::make_unique<CancelButton>(bot)});
    button_list.insert({"confirm_upgrade_post",std::make_unique<ConfirmPostButton>(bot)});
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

ListBootstrap::ListBootstrap(dpp::cluster *bot) 
{
    m_list_list.insert({"map_list",std::make_unique<MapList>(bot)});
    m_list_list.insert({"upgrade_post_list",std::make_unique<UpgradePostList>(bot)});
}

List *ListBootstrap::Find(std::string list_name) 
{
    return m_list_list.at(list_name).get();
}

