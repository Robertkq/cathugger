#include "bot.h"
#include <iostream>

cathugger::cathugger() : bot(std::getenv("BOT_TOKEN")) { register_commands(); }

void cathugger::start() {
  std::cout << "Bot is starting..." << std::endl;
  bot.start(dpp::st_wait);
}

void cathugger::stop() { std::cout << "Bot is stopping..." << std::endl; };

void cathugger::say_skibidi() { bot.message_create(dpp::message("Skibidi!")); }

void cathugger::register_commands() {
  bot.on_slashcommand([this](const dpp::slashcommand_t &event) {
    if (event.command.get_command_name() == "skibidi") {
      event.reply("Skibidi is here!");
    }
  });

  bot.on_ready([this](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {
      bot.global_command_create(
          dpp::slashcommand("skibidi", "Toilet", bot.me.id));
    }
  });
}