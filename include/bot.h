#pragma once

#include <dpp/dpp.h>

class cathugger {
public:
  cathugger();

public:
  void start();
  void stop();
  void say_skibidi();

private:
  void register_commands();

public:
  dpp::cluster bot;
};