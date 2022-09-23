#pragma once
#include <command.h>

class test1 : public Command
{
    public:
        void CreateCommand(dpp::commandhandler *handler);
        test1();
};

