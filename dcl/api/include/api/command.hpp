#pragma once
#include <dpp.h>
#include <memory>
#include <api/request.hpp>

class Command 
{
public:
    dpp::slashcommand command;
    bool isGlobal;
    virtual void HandleCommand(const dpp::slashcommand_t *event) = 0;
};

class StartCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "start";
    StartCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Start new discorp session",appid);
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        dpp::user target = event->command.get_issuing_user();
        CreateGameRequest request{target.id,target.username};
        dpp::message m;
        request.FillResponse(m);
        /*if (result == gm::Errors::SUCCESS) 
        {
            message = "Account succesfully loaded! Enjoy your stay on Discland! You can start your adventure by using /photo and see your surroundings!";
        } else {
            message = "Could not start a new account";
        } */

        event->reply(m);
    }

};