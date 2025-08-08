#include "utils.h"
#include <cstdlib>

bool check_env() {
  const char *token = std::getenv("BOT_TOKEN");
  return token != nullptr;
}