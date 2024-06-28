#pragma once

#include "logera/string_cast/string_cast.hpp"

namespace pmt
{

template <string_castable... TS>
auto str_cat(TS&&... first_) -> std::string;

} // namespace pmt

#include "str_cat-inl.hpp"