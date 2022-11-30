#pragma once
#include <vector>
#include <dpp.h>
#include "command.hpp"

class CommandBootstrap 
{
private:
    dpp::cluster *bot;
    std::vector<Command *> command_list;
public:
    CommandBootstrap(dpp::cluster *bot);
    void RegisterCommands();
    Command *Find(std::string command_name);
};