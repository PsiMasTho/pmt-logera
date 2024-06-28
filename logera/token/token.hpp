#pragma once

#include <cstdint>
#include <string>

namespace pmt
{

using colno_type  = std::size_t;
using lineno_type = std::size_t;
using lexeme_type = std::string;

struct token
{
  lexeme_type _lexeme;
  colno_type  _colno;
  lineno_type _lineno;
};

} // namespace pmt