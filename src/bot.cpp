#include "bot.h"
#include <iostream>

cathugger::cathugger() : bot(std::getenv("BOT_TOKEN")) {}
void cathugger::start() { bot.start(dpp::st_wait); }
void cathugger::stop(){};
void cathugger::say_skibidi() { bot.message_create(dpp::message("Skibidi!")); }