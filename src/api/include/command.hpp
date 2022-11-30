#pragma once
#include <dpp.h>
#include <memory>

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
                        add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
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