#pragma once
#include <dpp.h>
#include <memory>
#include <game_manager.hpp>

class Command 
{
public:
    dpp::slashcommand command;
    bool isGlobal;
    virtual void HandleCommand(const dpp::slashcommand_t *event) = 0;
};

class LoginCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "login";
    LoginCommand(dpp::snowflake appid)
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Logins to the cropbash",appid);
        command.add_option
                (
                    dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
                        add_choice(dpp::command_option_choice("Horse", std::string("animal_horse"))).
                        add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
                        add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
                )
            )
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        
        std::string animal = std::get<std::string>(event->get_parameter("animal"));
        event->reply(std::string("Blep! You chose") + animal);
    }
};

class StartCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "login";
    StartCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Start new discorp session",appid);
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        dpp::user target = event->command.get_issuing_user();
        gm::CreateGame(target.id,target.username);
    }

};