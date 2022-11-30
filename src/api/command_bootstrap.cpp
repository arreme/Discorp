#include <command_bootstrap.hpp>

CommandBootstrap::CommandBootstrap(dpp::cluster *bot) 
{
    this->bot = bot;
    command_list.push_back(new LoginCommand(bot->me.id));
}

void CommandBootstrap::RegisterCommands() 
{
    for(std::vector<Command *>::iterator it = command_list.begin(); it != command_list.end(); ++it) 
    {
        if ((*it)->isGlobal) 
        {
            bot->global_command_create((*it)->command);
        }
    }
}

Command *CommandBootstrap::Find(std::string command_name) 
{
    return command_list[0];
}