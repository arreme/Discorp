#include <command_bootstrap.hpp>

CommandBootstrap::CommandBootstrap(dpp::cluster *bot) 
{
    this->bot = bot;
    command_list.insert({"login",new LoginCommand(bot->me.id)});
}

void CommandBootstrap::RegisterCommands() 
{
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