#include <dpp/dpp.h>
#include <gd.h>
#include <stdio.h>
#include <send_image.h>

#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

const std::string BOT_TOKEN = "OTk5NjAyMDc1OTAwMDU1NjMz.GbkJI5.xcgdebavt3vZw827Z3dMnLg0m3fwSytJdi4COA";

int main()
{

    mongocxx::instance inst{};
    const auto uri = mongocxx::uri{"mongodb://localhost:27017"};
    mongocxx::client conn{uri};
    mongocxx::database db = conn["firstdb"];
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
        << "name" << "MongoDB3"
        << "type" << "database"
        << "count" << 1
        << "versions" << bsoncxx::builder::stream::open_array
            << "v3.2" << "v3.0" << "v2.6"
        << close_array
        << "info" << bsoncxx::builder::stream::open_document
            << "x" << 203
            << "y" << 102
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;
    auto col = db["user"];
    col.insert_one(doc_value.view());
    /* Create bot cluster */
    uint64_t intents = dpp::i_default_intents | dpp::i_message_content;
    dpp::cluster bot(BOT_TOKEN,intents);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    dpp::commandhandler command_handler(&bot);
    command_handler.add_prefix("/");
    SendImage sendimg;
    /* Register slash command here in on_ready */
    bot.on_ready([&command_handler,&sendimg](const dpp::ready_t& event) {
        sendimg.CreateCommand(&command_handler);
        command_handler.register_commands();
    });

    /* Start the bot */
    bot.start(false);

    return 0;
}
