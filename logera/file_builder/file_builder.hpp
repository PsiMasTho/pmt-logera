#pragma once

#include "logera/fw_decl/fw_decl.hpp"
#include "logera/multifile/multifile.hpp"
#include "logera/token/token.hpp"

#include <filesystem>
#include <optional>
#include <vector>
#include <variant>
#include <cstdint>

NS_FW_DECL_CLASS(pmt, i_warning_collector)

namespace pmt
{

class file_builder
{
public:
  file_builder(i_warning_collector& warning_collector_, std::filesystem::path filepath_);

  void begin_entry();
  void begin_attribute_declaration();
  void begin_variable_declaration();

  void push_date(token date_);
  void push_identifier(token identifier_);
  void push_regex(token regex_);
  void push_attribute_value(token value_);

  auto get_result() -> std::optional<std::variant<log_file, header_file>>;

private:
  void do_begin_entry();
  void do_begin_attribute_declaration();
  void do_begin_variable_declaration();

  void do_push_date(token& date_);
  void do_push_identifier(token& identifier_);
  void do_push_regex(token& regex_);
  void do_push_attribute_value(token& value_);

  std::uint8_t _state;    // begin_X
  std::uint8_t _previous; // the last action
  std::uint8_t _all;      // all actions encountered
  std::uint8_t _filetype; // e.g. log_file, header_file

  i_warning_collector& _warning_collector;

  std::filesystem::path              _filepath;

  std::optional<token>               _date;
  std::vector<entry>                 _entries;
  std::vector<attribute_declaration> _attributes;
  std::vector<variable_declaration>  _variables;
};

} // namespace pmt