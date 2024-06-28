#include "basename_from_path.hpp"

namespace pmt
{

auto basename_from_path(path_view path_) -> path_view
{
  auto const pos = path_.find_last_of("/\\");

  if (pos == path_view::npos)
    return path_;

  return path_.substr(pos + 1);
}

} // namespace pmt
