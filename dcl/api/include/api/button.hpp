#pragma once
#include <dpp.h>
#include <memory>
#include <api/request.hpp>

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

class UpgradePostButton : public Button 
{
public:
    UpgradePostButton(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            UpgradePostRequest request{target.id,std::stoi(commands.at(2)),PBUpgradeType::CAPACITY};
            dpp::message m;
            request.FillRequest(m);
            event.reply(m);
        }
    }
};
