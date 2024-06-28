#pragma once

#include <filesystem>
#include <string_view>

namespace pmt
{

using path_view = std::basic_string_view<std::filesystem::path::value_type>;

}