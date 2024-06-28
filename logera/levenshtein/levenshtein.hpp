#pragma once

#include <string_view>
#include <vector>

namespace pmt
{

class levenshtein
{
  std::vector<std::size_t> _matrix;

public:
  [[nodiscard]] auto distance(std::string_view lhs_, std::string_view rhs_) -> std::size_t;
};

} // namespace pmt