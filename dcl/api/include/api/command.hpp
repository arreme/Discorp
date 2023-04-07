#pragma once
#include <dpp.h>
#include <memory>
#include <core/game_manager.hpp>

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
        auto result = gm::CreateGame(target.id,target.username);
        if (result == gm::Errors::SUCCESS) 
        {
            event->reply("Account succesfully loaded! Enjoy your stay on Discland! You can start your adventure by using /photo and see your surroundings!");
        }
    }

};

class PhotoCommand : public Command
{
public:
    const std::string COMMAND_NAME = "photo";
    PhotoCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Print state",appid);
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        auto image_data = gm::PhotoCurrentLocation(target.id, &size);
        dpp::message m("");
        std::string s(image_data.get(),size);
        m.add_file("img1.png",s);
        event->reply(m);
    }
};

class InventoryCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "inventory";
    InventoryCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Print inventory",appid);
        command.add_option(
            dpp::command_option(dpp::co_string, "inventory_page", "The page of the inventory you want to look", true).
                add_choice(dpp::command_option_choice("Resources", std::string("resources"))).
                add_choice(dpp::command_option_choice("Armor", std::string("armor"))).
                add_choice(dpp::command_option_choice("Quest Items", std::string("quest_items"))).
                add_choice(dpp::command_option_choice("Consumables", std::string("utilities")))
        );

        command.add_option(
            dpp::command_option(dpp::co_integer, "page_number", "The number of the page you want to look at", false)
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        std::string page_type = std::get<std::string>(event->get_parameter("inventory_page"));
        auto page_variant = event->get_parameter("page_number");
        int page_number = 0;
        if (page_variant.index() != 0) 
        {
            page_number = static_cast<int>(std::get<std::int64_t>(page_variant));
        }
        auto image_data = gm::Inventory(target.id, page_type, page_number, &size);
        dpp::message m("");
        std::string s(image_data.get(),size);
        m.add_file("img1.png",s);
        m.add_component(
            dpp::component().add_component(
                dpp::component().set_label("<-").set_style(dpp::cos_primary).set_id("previous_page")
            ).add_component(
                dpp::component().set_label("->").set_style(dpp::cos_primary).set_id("next_page")
            )
        );
        event->reply(m);
    }
};


class UnlockZoneCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "unlockzone";
    UnlockZoneCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Unlocking access to the next zone!",appid);
        command.add_option(
            dpp::command_option(dpp::co_integer, "zone_acess_target", "The zone access you want to unlock", true)
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        std::string message = "MATERIALS REQUIRED\n-----------------\n";
        int interaction_number = static_cast<int>(std::get<std::int64_t>(event->get_parameter("zone_acess_target")));
        auto errors = gm::CanUnlock(target.id,interaction_number, message);
        
        if (errors == gm::Errors::ILLEGAL_ACTION || errors == gm::Errors::INTERACTION_NOT_FOUND) 
        {
            message = "Target zone could not be found or is not of type zone access";
        }
        dpp::message m(message);
        if (errors == gm::Errors::SUCCESS) 
        {
            m.add_component(
                dpp::component().add_component(
                    dpp::component().set_label("CONFIRM").set_style(dpp::cos_success).set_id("confirm_unlock")
                ).add_component(
                    dpp::component().set_label("CANCEL").set_style(dpp::cos_danger).set_id("cancel_unlock")
                )
            );
        }
        event->reply(m);
    }
};