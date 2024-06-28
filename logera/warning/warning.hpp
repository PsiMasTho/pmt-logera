#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace pmt
{

struct warning
{
  struct location
  {
    std::optional<std::filesystem::path> _filepath = std::nullopt;
    std::optional<int>                   _lineno   = std::nullopt;
    std::optional<int>                   _colno    = std::nullopt;
  };

  location    _location;
  std::string _msg;
  int         _code;
};

} // namespace pmt