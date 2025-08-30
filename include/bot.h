#pragma once

#include "songs.h"
#include <dpp/dpp.h>

class cathugger {
public:
  cathugger();

public:
  void start();
  void stop();

private:
  void register_slashcommands();
  std::vector<dpp::slashcommand> get_all_slashcommands();

  dpp::slashcommand my_join_slashcommand();
  dpp::slashcommand my_leave_slashcommand();
  dpp::slashcommand my_play_slashcommand();
  dpp::slashcommand my_playground_slashcommand();
  dpp::slashcommand my_stalk_slashcommand();
  dpp::slashcommand my_nostalk_slashcommand();

  void on_slashcommands();
  void on_voice_state_update();

  void join_voice_channel(const dpp::slashcommand_t &event);
  void leave_voice_channel(const dpp::slashcommand_t &event);
  void play_song(const dpp::slashcommand_t &event);
  void playground(const dpp::slashcommand_t &event);
  void stalk_user(const dpp::slashcommand_t &event);
  void nostalk_user(const dpp::slashcommand_t &event);

  constexpr std::vector<dpp::snowflake> supported_guilds() {
    dpp::snowflake robert_server = 1020792384424005743;
    dpp::snowflake make_money_not_friends_server = 415546041476186112;
    return {robert_server, make_money_not_friends_server};
  }

public:
  dpp::cluster bot;
  std::vector<song> songs;
  const std::vector<dpp::snowflake> guilds;
  std::atomic<bool> stalk;
  dpp::snowflake stalked_user_id;
};