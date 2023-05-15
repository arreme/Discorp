#pragma once
#include <vector>
#include <dpp.h>
#include <api/command.hpp>
#include <api/button.hpp>
#include <api/command.hpp>
#include <api/list.hpp>

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


class ButtonBootstrap 
{
private:
    std::map<std::string,std::unique_ptr<Button>> button_list;
public:
    ButtonBootstrap(dpp::cluster *bot);
    Button *Find(std::string button_name);
};

class ListBootstrap 
{
private:
    dpp::cluster *bot;
    std::map<std::string,std::unique_ptr<List>> m_list_list;
public:
    ListBootstrap(dpp::cluster *bot);
    List *Find(std::string list_name);
};