#pragma once

#include "logera/path_view/path_view.hpp"

#include <string_view>

namespace pmt
{

class i_file_reader
{
public:
  virtual auto read(path_view path_) -> std::string_view = 0;
};

} // namespace pmt