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
            UpgradePostRequest request{target.id,std::stoi(commands.at(2)),-1};
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
            UpgradePostRequest request{target.id,std::stoi(commands.at(2)),std::stoi(commands.at(3))};
            std::string response;
            dpp::message m;
            m.set_flags(dpp::m_ephemeral);
            if (request.ConfirmRequest()) 
            {
                m.set_content("Post upgraded!");
            } else {
                m.set_content("You don't have enough materials!");
            }
            event.reply(dpp::interaction_response_type::ir_update_message, m);
        }
    }
};

class UnlockLocation : public Button 
{
public:
    UnlockLocation(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            UnlockLocationRequest request{target.id,std::stoi(commands.at(2))};
            if (request.ConfirmRequest()) 
            {
                dpp::message m;
                PrintMapRequest map_request{target.id,std::stoi(commands.at(2))};
                map_request.FillRequest(m);
                event.reply(dpp::interaction_response_type::ir_update_message, m);
            } else {
                event.reply("You don't have enough materials!");
            }
            
        }
    }
};

class GoToLocation : public Button 
{
public:
    GoToLocation(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            GoToLocationRequest request{target.id,std::stoi(commands.at(2))};
            request.ConfirmRequest();
            PrintMapRequest map_request{target.id,-1};
            dpp::message m;
            map_request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message, m);
        }
    }
};

class CollectPost : public Button 
{
public:
    CollectPost(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            CollectPostRequest request{target.id,std::stoi(commands.at(2))};
            dpp::message m;
            request.ConfirmRequest();
            request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message, m);
        }
    }
};


class InventoryButton : public Button 
{
public:
    InventoryButton(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            int item_type = 0;
            int page = 0;
            if (commands.size() == 4) 
            {
                item_type = commands.size() < 3 ? 0 : std::stoi(commands.at(2));
                page = commands.size() < 3 ? 0 : std::stoi(commands.at(3));
            }
            
            PrintInventoryRequest request{target.id,item_type,page};
            dpp::message m;
            request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message, m);
        }
    }
};

class ProfileButton : public Button 
{
public:
    ProfileButton(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            PrintProfileRequest request{target.id};
            dpp::message m;
            request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message, m);
        }
    }
};

class PrintMap : public Button 
{
public:
    PrintMap(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            PrintMapRequest map_request{target.id,-1};
            dpp::message m;
            map_request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message, m);
        }
    }
};

class DialogInteraction : public Button 
{
public:
    DialogInteraction(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            ConversationRequest request{target.id,std::stoi(commands.at(2))};
            dpp::message m;
            request.FillRequest(m);
            event.reply(m);
        }
    }
};

class AcceptDuel : public Button 
{
public:
    AcceptDuel(dpp::cluster *bot)
    : Button(bot)
    {

    };

    void HandleButton(const dpp::button_click_t & event, std::vector<std::string> &commands) override 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(2) || std::to_string(target.id) == commands.at(1)) 
        {
            dpp::message m;
            PBCombatActions action = PBCombatActions::CA_NONE;
            if (commands.size() == 5) 
            {
                PBCombatActions_Parse(commands.at(4),&action); 
            }
            CombatRequest request{std::stoull(commands.at(1).data()),std::stoull(commands.at(2).data()),std::atoi(commands.at(3).data()),action,target.id};
            if (!request.FillRequest(m)) 
            {
                event.reply(m);
            } else {
                event.reply(dpp::interaction_response_type::ir_update_message, m);
            }
            
        } else {
            dpp::message m;
            m.set_flags(dpp::m_ephemeral);
            m.set_content("Not your combat!");
            event.reply(m);
        }
    }
};