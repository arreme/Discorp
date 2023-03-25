#pragma once
#include <dpp.h>
#include <memory>
#include <core/game_manager.hpp>

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
        auto result = gm::CreateGame(target.id,target.username);
        if (result == gm::Errors::SUCCESS) 
        {
            event->reply("Account succesfully loaded! Enjoy your stay on Discland! You can start your adventure by using /photo and see your surroundings!");
        }
    }

};

class PhotoCommand : public Command
{
public:
    const std::string COMMAND_NAME = "photo";
    PhotoCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Print state",appid);
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        auto image_data = gm::PhotoCurrentLocation(target.id, &size);
        dpp::message m("");
        std::string s(image_data.get(),size);
        m.add_file("img1.png",s);
        event->reply(m);
    }
};