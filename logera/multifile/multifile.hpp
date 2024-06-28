#pragma once

#include "logera/token/token.hpp"

#include <filesystem>
#include <vector>

namespace pmt
{

struct identifier_value_pair
{
  token _identifier;
  token _value;
};

struct identifier_value_pair_list
{
  std::vector<identifier_value_pair> _pairs;
};

struct entry
{
  token                      _entry_identifier;
  identifier_value_pair_list _pairs;
};

struct attribute_declaration
{
  token              _attribute_identifier;
  std::vector<token> _values;
};

struct variable_declaration
{
  token              _variable_identifier;
  std::vector<token> _values;
};

struct header_file
{
  std::filesystem::path              _filepath;
  std::vector<attribute_declaration> _attributes;
  std::vector<variable_declaration>  _variables;
};

struct log_file
{
  std::filesystem::path _filepath;
  token                 _date;
  std::vector<entry>    _entries;
};

struct multifile
{
  std::vector<header_file> _headers;
  std::vector<log_file>    _logs;
};

} // namespace pmt