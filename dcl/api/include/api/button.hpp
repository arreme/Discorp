#pragma once
#include <dpp.h>
#include <memory>
#include <core/game_manager.hpp>

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

class NextPageButton : public Button
{
public:
    NextPageButton(dpp::cluster *bot)
    : Button(bot) 
    {}
    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto user = event.command.get_issuing_user();
        //gm::NextPage(event);
    }
};

class PreviousPageButton : public Button 
{
public:
    PreviousPageButton(dpp::cluster *bot)
    : Button(bot) 
    {};

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {

    }
};

class ConfirmUnlockButton : public Button 
{
public:
    ConfirmUnlockButton(dpp::cluster *bot)
    : Button(bot) 
    {};

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        dpp::message m;
        std::string response;
        if (std::to_string(target.id) == commands.at(1)) 
        {
            response = "Confirming action...\n";
            int32_t interaction_num = std::stoi(commands.at(2));
            auto error = gm::UnlockZone(target.id,interaction_num);
            m_bot->message_delete(event.command.message_id,event.command.channel_id);
            if (error==gm::Errors::SUCCESS) 
            {
                response += "Zone Unlocked! You can now go to that zone with /goto "+commands.at(2);
            } else {
                response += "An error ocurred, please try again later";
            }
        } else 
        {
            response = "Can't use this button as you are not the target user!";
        }
        m.set_content(response).set_flags(dpp::m_ephemeral);
        event.reply(dpp::interaction_response_type::ir_update_message,m);
    }
};

class CancelUnlockButton : public Button 
{
public:
    CancelUnlockButton(dpp::cluster *bot)
    : Button(bot) 
    {};

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        std::string response;
        if (std::to_string(target.id) == commands.at(1)) 
        {
            //m_bot->message_delete(event.command.message_id,event.command.channel_id);
            response += "Canceled action. New zone not unlocked";
        } else 
        {
            response = "Can't use this button as you are not the target user!";
        }
        event.reply(dpp::interaction_response_type::ir_update_message,response);
    }
};