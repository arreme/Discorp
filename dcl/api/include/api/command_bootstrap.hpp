#pragma once
#include <vector>
#include <dpp.h>
#include <api/command.hpp>

class CommandBootstrap 
{
private:
    dpp::cluster *bot;
    std::map<std::string,Command *> command_list;
public:
    CommandBootstrap(dpp::cluster *bot);
    void RegisterCommands();
    Command *Find(std::string command_name);
};