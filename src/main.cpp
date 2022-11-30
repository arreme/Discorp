#include <dpp/dpp.h>
#include <gd.h>
#include <stdio.h>
#include <cstdint>
#include "command_bootstrap.hpp"

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
    uint64_t intents = dpp::i_default_intents | dpp::i_message_content;
    dpp::cluster bot(BOT_TOKEN,intents);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());
    /* Register slash command here in on_ready */
    CommandBootstrap bootstrap = CommandBootstrap(&bot);
    bot.on_ready([&bot,&bootstrap,delete_commands](const dpp::ready_t& event) {
        if (delete_commands) 
        {
            auto commands = bot.global_commands_get_sync();
            for (dpp::slashcommand_map::iterator it = commands.begin();
                    it != commands.end(); ++it)
            {
                bot.global_command_delete_sync(it->first);
            }
            
        }
        bootstrap.RegisterCommands();
    });

    bot.on_slashcommand([&bot,&bootstrap](const dpp::slashcommand_t & event) {
        Command *command = bootstrap.Find(event.command.get_command_name());
        command->HandleCommand(&event);
    });

    /* Start the bot */
    bot.start(false);

    return 0;
}
