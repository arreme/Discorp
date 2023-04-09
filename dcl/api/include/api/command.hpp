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
        std::string message;
        if (result == gm::Errors::SUCCESS) 
        {
            message = "Account succesfully loaded! Enjoy your stay on Discland! You can start your adventure by using /photo and see your surroundings!";
        } else {
            message = "Could not start a new account";
        }

        event->reply(message);
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
        m.set_flags(dpp::m_ephemeral);
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
        bool is_last_page = false;
        auto image_data = gm::Inventory(target.id, page_type, page_number, &size, is_last_page);
        
        dpp::message m("");
        if (size == -1) 
        {
            m.set_content("You need to initialize a game first! Use /start");
            event->reply(m);
            return;
        }
        std::string s(image_data.get(),size);
        m.add_file("img1.png",s);
        auto buttons = dpp::component();
        bool add_component = false;

        if (!is_last_page) 
        {
            add_component = true;
            buttons.add_component(dpp::component().set_label("->").set_style(dpp::cos_primary).set_id("next_page::"+std::to_string(page_number+1)));
        }
        if (page_number != 0) 
        {
            add_component = true;
            buttons.add_component(dpp::component().set_label("<-").set_style(dpp::cos_primary).set_id("previous_page::"+std::to_string(page_number-1)));
        }

        if (add_component) 
        {
            m.add_component(buttons);
        }
        m.set_flags(dpp::m_ephemeral);
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
        interaction_number--;
        auto errors = gm::CanUnlock(target.id,interaction_number, message);
        
        if (errors == gm::Errors::ILLEGAL_ACTION || errors == gm::Errors::INTERACTION_NOT_FOUND) 
        {
            message = "Target zone could not be found or is not of type zone access";
        } else if (errors == gm::Errors::INTERACTION_ALREADY_UNLOCKED) 
        {
            message = "Location is already unlocked!";
        }
        dpp::message m(message);
        if (errors == gm::Errors::SUCCESS) 
        {
            m.add_component(
                dpp::component().add_component(
                    dpp::component().set_label("CONFIRM").set_style(dpp::cos_success).set_id("confirm_unlock::"+std::to_string(target.id)+"::"+std::to_string(interaction_number))
                ).add_component(
                    dpp::component().set_label("CANCEL").set_style(dpp::cos_danger).set_id("cancel_unlock::"+std::to_string(target.id))
                )
            );
        }
        m.set_flags(dpp::m_ephemeral);
        event->reply(m);
    }
};

class GotoCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "goto";
    GotoCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"With this command you can go to the zone via a zone access",appid);
        command.add_option(
            dpp::command_option(dpp::co_integer, "zone_acess_target", "The zone access you want to go to", true)
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        int interaction_number = static_cast<int>(std::get<std::int64_t>(event->get_parameter("zone_acess_target")));
        interaction_number--;
        auto errors = gm::GoToZone(target.id,interaction_number);
        std::string message = "Changed zone!";
        if (errors == gm::Errors::ILLEGAL_ACTION || errors == gm::Errors::INTERACTION_NOT_FOUND) 
        {
            message = "Target zone could not be found or is not of type zone access";
        } else if (errors == gm::Errors::LOCATION_LOCKED) 
        {
            message = "Location is not unlocked! You should try to repare the path with your given materials!";
        }
        dpp::message m(message);
        m.set_flags(dpp::m_ephemeral);
        event->reply(m);
    }
};

class CollectCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "collect";
    CollectCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Collect target post so you can obtain it's resources",appid);
        command.add_option(
            dpp::command_option(dpp::co_integer, "post_target", "The post you want to collect", true)
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        int interaction_number = static_cast<int>(std::get<std::int64_t>(event->get_parameter("post_target")));
        interaction_number--;
        std::string message;
        auto errors = gm::CollectPost(target.id,interaction_number, message);
        dpp::message m(message);
        m.set_flags(dpp::m_ephemeral);
        event->reply(m);
    }
};

class ImproveCommand : public Command 
{
public:
    const std::string COMMAND_NAME = "improve";
    ImproveCommand(dpp::snowflake appid) 
    {
        isGlobal = true;
        command = dpp::slashcommand(COMMAND_NAME,"Improve the target post with one of the properties specified",appid);
        command.add_option(
            dpp::command_option(dpp::co_integer, "post_target", "The post you want to collect", true)
        );

        command.add_option(
            dpp::command_option(dpp::co_string, "post_property", "The property of the post you want to improve", true).
                add_choice(dpp::command_option_choice("Capacity", std::string("CAPACITY"))).
                add_choice(dpp::command_option_choice("Generation per Second", std::string("GEN_SECOND"))).
                add_choice(dpp::command_option_choice("Fortune", std::string("FORTUNE")))
        );
    }

    void HandleCommand(const dpp::slashcommand_t *event) 
    {
        int size = -1;
        dpp::user target = event->command.get_issuing_user();
        int interaction_number = static_cast<int>(std::get<std::int64_t>(event->get_parameter("zone_acess_target")));
        interaction_number--;
        auto errors = gm::GoToZone(target.id,interaction_number);
        std::string message = "Changed zone!";
        if (errors == gm::Errors::ILLEGAL_ACTION || errors == gm::Errors::INTERACTION_NOT_FOUND) 
        {
            message = "Target zone could not be found or is not of type zone access";
        } else if (errors == gm::Errors::LOCATION_LOCKED) 
        {
            message = "Location is not unlocked! You should try to repare the path with your given materials!";
        }
        dpp::message m(message);
        m.set_flags(dpp::m_ephemeral);
        event->reply(m);
    }
};