#pragma once
#include <command.h>

class SendImage : public Command
{
    public:
        void CreateCommand(dpp::commandhandler *handler);
        SendImage();
    private:
        FILE *background1;
        gdImagePtr ptr1;
};

