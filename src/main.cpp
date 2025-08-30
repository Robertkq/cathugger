#include "bot.h"
#include "utils.h"

int main(int argc, char **argv) {

  if (!check_env()) {
    std::cerr << "Environment variable BOT_TOKEN is not set." << std::endl;
    return 1;
  }
  cathugger bot;
  bot.start();

  std::cout << "Press Enter to stop the bot..." << std::endl;
  std::cin.get();

  bot.stop();
  return 0;
}
