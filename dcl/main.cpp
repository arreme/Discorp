#include <dpp/dpp.h>
#include <stdio.h>
#include <cstdint>
#include <db/db_instance.hpp>
#include <api/bootstraps.hpp>

const std::string BOT_TOKEN = "OTk5NjAyMDc1OTAwMDU1NjMz.GbkJI5.xcgdebavt3vZw827Z3dMnLg0m3fwSytJdi4COA";

int main(int argc, char *argv[])
{
    bool delete_commands = false;
    if (argc>1) {
        char* option = argv[1];
        if (option[0]=='d') {
            delete_commands = true;
        }
    }

    db::MongoDBInstance::GetInstance()->createPool("mongodb://192.168.0.11:27017");
    
    uint64_t intents = dpp::i_default_intents | dpp::i_message_content;
    dpp::cluster bot(BOT_TOKEN,intents);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());
    /* Register slash command here in on_ready */
    CommandBootstrap bootstrap = CommandBootstrap(&bot);
    ButtonBootstrap button_bootstrap = ButtonBootstrap(&bot);
    ListBootstrap list_bootstrap = ListBootstrap(&bot);
    //ButtonBootstrap button_bootstrap{&bot};
    bot.on_ready([&bot,&bootstrap,delete_commands](const dpp::ready_t& event) {
        if (delete_commands) 
        {
            auto commands = bot.global_commands_get_sync();
            for (auto & command : commands)
            {
                bot.global_command_delete_sync(command.first);
            }
            bootstrap.RegisterCommands();
        }
    });

    bot.on_slashcommand([&bot,&bootstrap](const dpp::slashcommand_t & event) {
        Command *command = bootstrap.Find(event.command.get_command_name());
        command->HandleCommand(&event);
    });

    bot.on_select_click([&bot, &list_bootstrap](const dpp::select_click_t & event) {

        std::vector<std::string> list_commands;
        size_t pos = 0;
        std::string token;
        std::string custom_id_mutable = event.custom_id;
        std::string delimiter = "::";
        while ((pos = custom_id_mutable.find(delimiter)) != std::string::npos) {
            token = custom_id_mutable.substr(0, pos);
            list_commands.push_back(token);
            custom_id_mutable.erase(0, pos + delimiter.length());
        }
        list_commands.push_back(custom_id_mutable);
        auto list = list_bootstrap.Find(list_commands.at(0));
        list->HandleList(event, list_commands);
    });

    bot.on_button_click([&bot,&button_bootstrap](const dpp::button_click_t & event) {
        std::vector<std::string> button_commands;
        size_t pos = 0;
        std::string token;
        std::string custom_id_mutable = event.custom_id;
        std::string delimiter = "::";
        while ((pos = custom_id_mutable.find(delimiter)) != std::string::npos) {
            token = custom_id_mutable.substr(0, pos);
            button_commands.push_back(token);
            custom_id_mutable.erase(0, pos + delimiter.length());
        }
        button_commands.push_back(custom_id_mutable);
        auto button = button_bootstrap.Find(button_commands.at(0));
        button->HandleButton(event, button_commands);
    });

    /* Start the bot */
    bot.start(false);

    return 0;
}
