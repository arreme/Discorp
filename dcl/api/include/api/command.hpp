#pragma once
#include <dpp.h>
#include <memory>
#include <api/request.hpp>

class Command 
{
public:
    dpp::slashcommand command;
    bool isGlobal;
    virtual void HandleCommand(const dpp::slashcommand_t *event) = 0;
};

class StartCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "start";
    StartCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Start new discorp session",appid);
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        dpp::user target = event->command.get_issuing_user();
        CreateGameRequest request{target.id,target.username,event->command.guild_id};
        dpp::message m;
        int retries = 0;
        while (!request.FillResponse(m)) 
        {
            retries++;
            if (retries >= 3) 
            {
                m.set_content("There was an error, please try again later");
                return;
            }
        }
        event->reply(m);
    }

};

class DuelCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "duel";
    DuelCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Fight someone in an online battle!",appid);
        command.add_option(
            dpp::command_option(dpp::co_integer, "wager","The gold you want to wager in this duel.",true)
        );
        command.add_option(
            dpp::command_option(dpp::co_user,"opponent","The user you want to duel with",true)
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        dpp::user target = event->command.get_issuing_user();
        dpp::message m;
        int wager = static_cast<int>(std::get<std::int64_t>(event->get_parameter("wager")));
        dpp::snowflake opponent_id = std::get<dpp::snowflake>(event->get_parameter("opponent"));
        //m.set_content("Attention! <@"+std::to_string(opponent_id)+"> you just got challenged in a duel!");
        m.add_embed(
            dpp::embed().
            set_color(dpp::colors::scarlet_red).
            set_author(target.username,"",target.get_avatar_url()).
            set_description("Attention! <@!"+std::to_string(opponent_id)+"> you just got challenged in a duel!")
        );
        m.add_component(
            dpp::component().add_component(
                dpp::component().set_label("ACCEPT").set_id("accept_duel::"+std::to_string(wager)+"::"+std::to_string(target.id)+"::"+std::to_string(opponent_id)).set_style(dpp::cos_success)
            )
        );
        std::vector<dpp::snowflake> mentions = {opponent_id};
        m.set_allowed_mentions(false,false,false,false,mentions,{});
        event->reply(m);
    }
};