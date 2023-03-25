#include <api/command_bootstrap.hpp>

CommandBootstrap::CommandBootstrap(dpp::cluster *bot) 
{
    this->bot = bot;
    command_list.insert({"start",new StartCommand(bot->me.id)});
    command_list.insert({"photo",new PhotoCommand(bot->me.id)});
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