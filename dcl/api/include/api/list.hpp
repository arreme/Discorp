#pragma once
#include <dpp.h>
#include <memory>
#include <api/request.hpp>

class List 
{
protected:
    dpp::cluster *m_bot;
public:
    List(dpp::cluster *bot) 
    {
        m_bot = bot;
    };

    virtual void HandleList(const dpp::select_click_t & event, std::vector<std::string> &commands) = 0;
};

class MapList : public List 
{
public:
    MapList(dpp::cluster *bot)
    : List(bot)
    {}

    void HandleList(const dpp::select_click_t & event, std::vector<std::string> &commands) 
    {
        auto id = event.command.get_issuing_user().id;
        if (std::to_string(id) == commands.at(1)) 
        {
            PrintMapRequest map_request{id,std::stoi(event.values[0])};
            dpp::message m;
            map_request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message,m);
        }
    }
};

class UpgradePostList : public List
{
public:
    UpgradePostList(dpp::cluster *bot)
    : List(bot)
    {}

    void HandleList(const dpp::select_click_t & event, std::vector<std::string> &commands) 
    {
        auto target = event.command.get_issuing_user();
        if (std::to_string(target.id) == commands.at(1)) 
        {
            UpgradePostRequest request{target.id,std::stoi(commands.at(2)),static_cast<PBUpgradeType>(std::stoi(event.values[0]))};
            dpp::message m;
            request.FillRequest(m);
            event.reply(dpp::interaction_response_type::ir_update_message,m);
        }
    }
};