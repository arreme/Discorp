#pragma once
#include <dpp.h>
#include <memory>

class Button 
{
protected:
    dpp::cluster *m_bot;
public:
    Button(dpp::cluster *bot) 
    {
        m_bot = bot;
    };

    virtual void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) = 0;
};
