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

class CancelButton : public Button 
{
public:
    CancelButton(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        std::string response;
        if (std::to_string(target.id) == commands.at(1)) 
        {
            //m_bot->message_delete(event.command.message_id,event.command.channel_id);
            response += "Canceled action. Post not improved";
        } else 
        {
            response = "Can't use this button as you are not the target user!";
        }
        event.reply(dpp::interaction_response_type::ir_update_message,response);
    }
};

class ConfirmPostButton : public Button 
{
public:
    ConfirmPostButton(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            UpgradePostRequest request{target.id,std::stoi(commands.at(2)),PBUpgradeType::CAPACITY};
            std::string response;
            if (request.ConfirmRequest()) 
            {
                response = "Post upgraded!";
            } else {
                response = "You don't have enough materials!";
            }
            event.reply(dpp::interaction_response_type::ir_update_message, response);
        }
    }
};
