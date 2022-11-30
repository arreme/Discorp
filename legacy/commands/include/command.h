#pragma once
#include <dpp.h>
#include <gd.h>
class Command {
    public:
        virtual void CreateCommand(dpp::commandhandler *handler) = 0;
    protected:
        dpp::commandhandler *handler;
};