#ifndef KQBOT
#define KQBOT

#include <dpp/dpp.h>
#include "ImageRandomizer.h"

namespace kq
{
    struct Bot
    {
    public:
        Bot(const std::string& token, size_t nCats, size_t nDogs, uint32_t intents = dpp::i_default_intents);
        ~Bot();

        dpp::cluster& getCluster() { return m_cluster; }
        const dpp::cluster& getCluster() const { return m_cluster; }

        dpp::cluster& start(bool wait = dpp::st_wait) 
        {
            m_cluster.on_ready([this](const dpp::ready_t& event) {
                /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
                if (dpp::run_once<struct register_bot_commands>()) {

                    dpp::slashcommand cute("cute", "Get yourself a cute animal picture", m_cluster.me.id);
                    cute.add_option(
                        dpp::command_option(dpp::co_string, "animal", "type of animal", true).
                        add_choice(dpp::command_option_choice("cat", std::string("animal_cat"))).
                        add_choice(dpp::command_option_choice("dog", std::string("animal_dog")))
                    );

                    m_cluster.global_command_create(cute);
                }
                });
            m_cluster.start(wait);
            return m_cluster;
        }

        dpp::event_handle on_slashcommand(std::function<void(const dpp::slashcommand_t&)> func)
        {
            return m_cluster.on_slashcommand(func);
        }
        
        Bot(const Bot&) = delete;
        Bot& operator=(const Bot&) = delete;

    private:
        dpp::cluster m_cluster;
    };

    Bot::Bot(const std::string& token, size_t nCats, size_t nDogs, uint32_t intents)
        : m_cluster(token, intents)
    {
        std::cout << "Started bot...\n";
        ImageRandomizer::getInstance(nCats, nDogs); // Initialize the single ton with the proper arguments, further calls to getInstance() don't need any parameters
    }

    Bot::~Bot()
    {
        std::cout << "Closing bot...\n";
        delete ImageRandomizer::getInstance();
    }
}

#endif