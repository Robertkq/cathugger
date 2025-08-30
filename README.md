# Cathugger Discord Bot

Cathugger is a Discord bot written in C++ using the [DPP](https://dpp.dev/index.html) library.

## Features

- **Join voice chat:**  
  The bot can join your current voice channel.

- **Leave voice chat:**  
  The bot leaves the voice channel it is connected to.

- **Play:**  
  Play audio from a provided YouTube link in the voice channel.

- **Stalk:**  
  Make the bot automatically follow a specific user (`uid`) into any voice channel they join or switch to.

- **Nostalk:**  
  Stop the bot from following the specified user.

## Usage

Invite Cathugger to your server and use the following slash commands:

- `/join` — Bot joins your voice channel.
- `/leave` — Bot leaves the voice channel.
- `/play <YouTube link>` — Plays the audio from the provided YouTube link.
- `/stalk <uid>` — Bot will follow the user with the given ID into voice channels.
- `/nostalk` — Bot will stop following the user.