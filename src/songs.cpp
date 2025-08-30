#include "songs.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

std::vector<song> song::from_json(const std::string &filename) {
  std::vector<song> songs;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return songs;
  }

  json j;
  file >> j;

  for (const auto &item : j) {
    songs.push_back({item.value("title", ""), item.value("artist", ""),
                     item.value("provider", ""),
                     item.value("download_link", "")});
  }

  return songs;
}