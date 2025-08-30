#include "bot.h"
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <thread>
#include <vector>

using json = nlohmann::json;

cathugger::cathugger()
    : bot(std::getenv("BOT_TOKEN")),
      songs(song::from_json("../data/songs.json")), guilds(supported_guilds()) {
  bot.on_log(dpp::utility::cout_logger());

  register_slashcommands();
  on_slashcommands();
  on_voice_state_update();
}

void cathugger::start() {
  log("Bot is starting...\n");
  bot.start(dpp::st_return);
}

void cathugger::stop() {
  log("Bot is stopping...\n");
  bot.shutdown();
};

void cathugger::register_slashcommands() {
  bot.on_ready([this](const dpp::ready_t &event) {
    if (dpp::run_once<struct register_bot_commands>()) {

      std::vector<dpp::slashcommand> commands = get_all_slashcommands();

      for (const auto &guild : guilds) {
        for (const auto &command : commands) {
          bot.guild_command_create(command, guild);
        }
      }
      log("Slash commands registered successfully.\n");
    }
  });
}

std::vector<dpp::slashcommand> cathugger::get_all_slashcommands() {
  std::vector<dpp::slashcommand> commands;

  commands.emplace_back(my_join_slashcommand());
  commands.emplace_back(my_leave_slashcommand());
  commands.emplace_back(my_play_slashcommand());
  commands.emplace_back(my_playground_slashcommand());
  commands.emplace_back(my_stalk_slashcommand());
  commands.emplace_back(my_nostalk_slashcommand());

  return commands;
}

dpp::slashcommand cathugger::my_join_slashcommand() {
  return dpp::slashcommand("join", "Join a voice channel", bot.me.id);
}

dpp::slashcommand cathugger::my_leave_slashcommand() {
  return dpp::slashcommand("leave", "Leave the voice channel", bot.me.id);
}

dpp::slashcommand cathugger::my_play_slashcommand() {
  dpp::slashcommand play("play", "Play a song by url", bot.me.id);
  play.add_option(dpp::command_option(dpp::co_string, "url",
                                      "The URL of the song to play", true));

  return play;
}

dpp::slashcommand cathugger::my_playground_slashcommand() {
  return dpp::slashcommand("playground", "Testing stuff", bot.me.id);
}

dpp::slashcommand cathugger::my_stalk_slashcommand() {
  dpp::slashcommand stalk("stalk", "Stalk a user", bot.me.id);
  stalk.add_option(
      dpp::command_option(dpp::co_user, "user", "The user to stalk", true));
  return stalk;
}

dpp::slashcommand cathugger::my_nostalk_slashcommand() {
  return dpp::slashcommand("nostalk", "Stop stalking a user", bot.me.id);
}

void cathugger::on_slashcommands() {
  bot.on_slashcommand([this](const dpp::slashcommand_t &event) {
    log("===== Received slash command =====\n");
    log("Command Name: {}\n", event.command.get_command_name());
    log("User ID: {}\n",
        static_cast<uint64_t>(event.command.get_issuing_user().id));
    log("Guild ID: {}\n", static_cast<uint64_t>(event.command.guild_id));
    log("Channel ID: {}\n", static_cast<uint64_t>(event.command.channel_id));

    std::string command_name = event.command.get_command_name();
    if (command_name == "join") {
      log("Received join command\n");
      join_voice_channel(event);
    } else if (command_name == "leave") {
      log("Received leave command\n");
      leave_voice_channel(event);
    } else if (command_name == "play") {
      log("Received play command\n");
      play_song(event);
    } else if (command_name == "playground") {
      log("Received playground command\n");
      playground(event);
    } else if (command_name == "stalk") {
      log("Received stalk command\n");
      stalk_user(event);
    } else if (command_name == "nostalk") {
      log("Received nostalk command\n");
      nostalk_user(event);
    } else {
      event.reply("Unknown command");
    }
  });
}

void cathugger::on_voice_state_update() {
  bot.on_voice_state_update([this](const dpp::voice_state_update_t &event) {
    dpp::voicestate vs = event.state;
    if (vs.user_id == stalked_user_id && vs.channel_id != 0) {
      // The target user joined a voice channel
      // Connect your bot to vs.channel_id
      bot.get_shard(0)->connect_voice(vs.guild_id, vs.channel_id, false, false,
                                      false);
    }
  });
}

void cathugger::join_voice_channel(const dpp::slashcommand_t &event) {

  log("Trying to join voice channel\n");

  auto *g = dpp::find_guild(event.command.guild_id);
  if (!g) {
    event.reply("Could not find guild data!");
    log("Guild not found: {}\n", static_cast<uint64_t>(event.command.guild_id));
    return;
  }

  auto it = g->voice_members.find(event.command.get_issuing_user().id);
  if (it == g->voice_members.end()) {
    event.reply("You are not in a voice channel!");
    log("User [{}] not in a voice channel!\n",
        static_cast<uint64_t>(event.command.get_issuing_user().id));
    return;
  }

  dpp::snowflake channel_id = it->second.channel_id;

  log("Found voice channel: {}\n", static_cast<uint64_t>(channel_id));

  dpp::discord_client &client =
      event.from()->connect_voice(g->id, channel_id, false, false, false);

  event.reply("Joining your channel!");
  log("Joining the channel\n");
}

void cathugger::leave_voice_channel(const dpp::slashcommand_t &event) {
  log("Trying to leave voice channel\n");

  event.from()->disconnect_voice(event.command.guild_id);

  event.reply("Leaving voice channel");
}

void cathugger::play_song(const dpp::slashcommand_t &event) {
  log("Trying to play a song\n");

  std::string song_url = std::get<std::string>(event.get_parameter("url"));
  log("Song URL: {}\n", song_url);

  dpp::voiceconn *v = event.from()->get_voice(event.command.guild_id);

  if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
    event.reply("I need to be in a voice channel to play music.");
    log("Voice channel not found");
    return;
  }

  event.thinking();

  std::string command = std::format("yt-dlp -x -f 251 --force-overwrites "
                                    "--no-playlist -o \"{}\" \"{}\"",
                                    "song", song_url);

  int sc = system(command.c_str());
  if (sc != 0) {
    event.edit_response("Failed to download the song. Please check the URL.");
    log("Failed to execute command: {}\n", command);
    return;
  }

  command =
      std::format("ffmpeg -y -i song.opus -f s16le -ar 48000 -ac 2 song.pcm");
  sc = system(command.c_str());
  if (sc != 0) {
    event.edit_response("Failed to convert the song. Please try again.");
    log("Failed to execute command: {}\n", command);
    return;
  }

  std::ifstream pcm_file("song.pcm", std::ios::binary);
  std::vector<uint8_t> pcm_data((std::istreambuf_iterator<char>(pcm_file)),
                                std::istreambuf_iterator<char>());

  // Truncate to ensure sample count is a multiple of 4
  size_t sample_count = pcm_data.size() / 2;
  if (sample_count % 4 != 0) {
    size_t new_size = (sample_count - (sample_count % 4)) * 2;
    log("Truncating last {} bytes to ensure sample count is multiple of 4\n",
        pcm_data.size() - new_size);
    pcm_data.resize(new_size);
    sample_count = pcm_data.size() / 2;
  }

  v->voiceclient->send_audio_raw(reinterpret_cast<uint16_t *>(pcm_data.data()),
                                 sample_count);

  event.edit_response(std::format("`Playing song`**:** {}", song_url));
}

void cathugger::playground(const dpp::slashcommand_t &event) {
  log("Playground command received\n");
  bot.on_voice_ready([this](const dpp::voice_ready_t &event) {
    log("Voice connection established by ID: {}\n",
        static_cast<uint64_t>(event.owner->me.id));
    log("On voice channel {}\n",
        static_cast<uint64_t>(event.voice_client->channel_id));
  });
  dpp::guild *g = dpp::find_guild(event.command.guild_id);
  const dpp::user &user = event.command.get_issuing_user();
  bool result = g->connect_member_voice(bot, user.id, false, false, false);
  log("Result of connect_member_voice is: {}\n", result);

  event.reply("Maybe I joined?");
}

void cathugger::stalk_user(const dpp::slashcommand_t &event) {
  dpp::snowflake user_id =
      std::get<dpp::snowflake>(event.get_parameter("user"));
  log("Stalking user: {}\n", static_cast<uint64_t>(user_id));
  stalked_user_id = user_id;
  stalk = true;
  event.reply("https://tenor.com/view/"
              "this-cat-i-have-hired-this-cat-to-stare-at-you-hired-cat-cat-"
              "stare-gif-26392360");
}

void cathugger::nostalk_user(const dpp::slashcommand_t &event) {
  log("No longer stalking user: {}\n", static_cast<uint64_t>(stalked_user_id));
  stalk = false;
  stalked_user_id = 0;
  event.reply("No longer hired to stare at you. meow");
}
