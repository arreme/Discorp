#include <dpp/dpp.h>
#include <gd.h>
#include <stdio.h>
#include <test1.h>
#include <send_image.h>

const std::string BOT_TOKEN = "OTk5NjAyMDc1OTAwMDU1NjMz.GbkJI5.xcgdebavt3vZw827Z3dMnLg0m3fwSytJdi4COA";

int main()
{
  /* Create bot cluster */
  uint64_t intents = dpp::i_default_intents | dpp::i_message_content;
  dpp::cluster bot(BOT_TOKEN,intents);

  /* Output simple log messages to stdout */
  bot.on_log(dpp::utility::cout_logger());

  dpp::commandhandler command_handler(&bot);
  command_handler.add_prefix("/");
  test1 com1;
  SendImage sendimg;
  /* Register slash command here in on_ready */
  bot.on_ready([&command_handler, &com1,&sendimg](const dpp::ready_t& event) {
      com1.CreateCommand(&command_handler);
      sendimg.CreateCommand(&command_handler);
      command_handler.register_commands();
  });

  /* Start the bot */
  bot.start(false);
  //change
  return 0;
}
