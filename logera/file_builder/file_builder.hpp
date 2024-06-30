#pragma once

#include "logera/fw_decl/fw_decl.hpp"
#include "logera/multifile/multifile.hpp"
#include "logera/token/token.hpp"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <variant>
#include <vector>

NS_FW_DECL_CLASS(pmt, i_warning_collector)

namespace pmt
{

class file_builder
{
public:
  file_builder(i_warning_collector& warning_collector_, std::filesystem::path filepath_);

  void end_date();
  void end_entry_identifier();
  void end_ivpl();
  void end_attribute_declaration();
  void end_variable_declaration();

  void enqueue_token(token token_);
  void clear_queue();

  auto get_result() -> std::optional<std::variant<log_file, header_file>>;

private:
  auto identify_filetype() -> std::uint8_t;
  auto assemble_log_file() -> log_file;
  auto assemble_header_file() -> header_file;

  std::vector<token> _queued_tokens;
  std::uint8_t       _all; // all actions encountered

  i_warning_collector& _warning_collector;

  std::filesystem::path _filepath;

  std::optional<token>               _date;
  std::vector<entry>                 _entries;
  std::vector<attribute_declaration> _attributes;
  std::vector<variable_declaration>  _variables;
};

} // namespace pmt