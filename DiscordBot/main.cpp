#include <dpp/dpp.h>
#include "ImageRandomizer.h"

/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Too many or too little arguments, only place the bot token as the single command line argument.\n";
        return 0;
    }
    const std::string BOT_TOKEN = argv[1];

    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN);

    ImageRandomizer::getInstance(3, 2);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Handle slash command */
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
        });

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        //event.command.get_channel().id
        dpp::message msg{""};
        if (event.command.get_command_name() == "cat") {
            msg.set_content("Meeow!");

            std::string filename = ImageRandomizer::getInstance()->getCat();

            msg.add_file(filename, dpp::utility::read_file("images/cats/" + filename));

            //bot.message_create(msg);
            event.reply(msg);
        }
        else if (event.command.get_command_name() == "dog") {
            msg.set_content("Woof!");

            std::string filename = ImageRandomizer::getInstance()->getDog();

            msg.add_file(filename, dpp::utility::read_file("images/dogs/" + filename));

            event.reply(msg);
        }
        });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("cat", "Get yourself a cute cat photo", bot.me.id));
            bot.global_command_create(dpp::slashcommand("dog", "Get yourself a cute dog photo", bot.me.id));
        }
        });

    /* Start the bot */
    bot.start(false);

    return 0;
}
