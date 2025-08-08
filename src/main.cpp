#include "bot.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (!check_env()) {
    std::cerr << "Environment variable BOT_TOKEN is not set." << std::endl;
    return 1;
  }
  cathugger bot;
  bot.start();
  bot.say_skibidi();
  bot.stop();
  return 0;
}
