#pragma once

#include <command.h>

/*
* Called when registering for the first time into the bot
*/
class Login : public Command
{
    public:
        void CreateCommand(dpp::commandhandler *handler);
        Login();
    private:

};