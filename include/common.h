#pragma once

#include <format>
#include <iostream>

#ifdef NDEBUG
constexpr bool is_debug_mode = false;
#else
constexpr bool is_debug_mode = true;
#endif

template <typename... Args> void log(const std::string &fmt, Args &&...args) {
  std::cout << std::vformat(fmt,
                            std::make_format_args(std::forward<Args>(args)...));
}