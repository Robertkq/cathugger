#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class song {
public:
  static std::vector<song> from_json(const std::string &filename);

public:
  std::string title;
  std::string artist;
  std::string provider;
  std::string download_link;
  std::string play_path;
};