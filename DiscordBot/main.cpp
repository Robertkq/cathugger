#include <dpp/dpp.h>
#include "ImageRandomizer.h"
#include "Bot.h"

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

    char ch = 'A';
    std::string str(std::to_string(ch));

    kq::Bot bot(argv[1], 3, 2);

    /* Output simple log messages to stdout */
    bot.getCluster().on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "cute")
        {
            std::string animal_type = std::get<std::string>(event.get_parameter("animal"));

            if (animal_type == "animal_cat")
            {
                dpp::message msg{"Meeow!"};
                std::string filename = kq::ImageRandomizer::getInstance()->getCat();

                msg.add_file(filename, dpp::utility::read_file("images/cats/" + filename));

                event.reply(msg);
            }
            else if (animal_type == "animal_dog")
            {
                dpp::message msg{ "Woof!" };
                std::string filename = kq::ImageRandomizer::getInstance()->getDog();

                msg.add_file(filename, dpp::utility::read_file("images/dogs/" + filename));

                event.reply(msg);
            }
        }
        });

    /* Start the bot */
    bot.start();

    return 0;
}
