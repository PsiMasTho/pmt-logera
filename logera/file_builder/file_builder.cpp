#include "file_builder.hpp"

#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/warning/warning.hpp"
#include "logera/warning_code/warning_code.hpp"

#include <cassert>
#include <variant>

namespace pmt
{

namespace
{

enum history : std::uint8_t
{
  NONE                      = 0,
  END_DATE                  = 1 << 0,
  END_ENTRY_IDENTIFIER      = 1 << 1,
  END_IVPL                  = 1 << 2,
  END_ATTRIBUTE_DECLARATION = 1 << 3,
  END_VARIABLE_DECLARATION  = 1 << 4
};

enum filetype : std::uint8_t
{
  INVALID     = 0,
  LOG_FILE    = 1 << 0,
  HEADER_FILE = 1 << 1
};

} // namespace

file_builder::file_builder(i_warning_collector& warning_collector_, std::filesystem::path filepath_)
  : _queued_tokens()
  , _all(NONE)
  , _warning_collector(warning_collector_)
  , _filepath(std::move(filepath_))
{
  _queued_tokens.reserve(16);
}

void file_builder::end_date()
{
  token& date = _queued_tokens.back();

  if (_all != NONE)
    _warning_collector.add_warning(
      warning_code::DATE_NOT_FIRST_IN_FILE,
      warning::location{ ._filepath = _filepath, ._lineno = date._lineno, ._colno = date._colno },
      "date must be the first token in the file");

  if (_date.has_value())
    _warning_collector.add_warning(
      warning_code::REDEFINING_DATE,
      warning::location{ ._filepath = _filepath, ._lineno = date._lineno, ._colno = date._colno },
      "redefining date");

  _date = std::move(date);

  _all |= END_DATE;
}

void file_builder::end_entry_identifier()
{
  token& entry_identifier = _queued_tokens.back();

  _entries.push_back(entry{ ._entry_identifier = std::move(entry_identifier) });

  _all |= END_ENTRY_IDENTIFIER;
}

void file_builder::end_ivpl()
{
  if (_entries.empty())
  {
    warning::location location{ ._filepath = _filepath };
    if (_queued_tokens.size() > 0)
    {
      location._lineno = _queued_tokens.front()._lineno;
      location._colno  = _queued_tokens.front()._colno;
    }

    _warning_collector.add_warning(
      warning_code::IVPL_WITHOUT_ENTRY,
      std::move(location),
      "identifier-value pair list without entry");
    return;
  }

  entry& current_entry = _entries.back();

  identifier_value_pair_list ivpl;
  ivpl._pairs.reserve(_queued_tokens.size());

  if (_queued_tokens.size() % 2 != 0)
  {
    warning::location location{ ._filepath = _filepath,
                                ._lineno   = _queued_tokens.back()._lineno,
                                ._colno    = _queued_tokens.back()._colno };
    _warning_collector.add_warning(
      warning_code::IVPL_UNPAIRED,
      std::move(location),
      "identifier-value pair list has unpaired tokens");

    // Try to recover
    _queued_tokens.pop_back();
  }

  for (std::size_t i = 0; i < _queued_tokens.size();)
    ivpl._pairs.push_back(
      identifier_value_pair{ ._identifier = std::move(_queued_tokens[i++]), ._value = std::move(_queued_tokens[i++]) });

  current_entry._pair_lists.push_back(std::move(ivpl));

  _all |= END_IVPL;
}

void file_builder::end_attribute_declaration()
{
  attribute_declaration attribute_declaration_;
  attribute_declaration_._attribute_identifier = std::move(_queued_tokens[0]);

  attribute_declaration_._values.reserve(_queued_tokens.size() - 1);

  for (std::size_t i = 1; i < _queued_tokens.size(); ++i)
    attribute_declaration_._values.push_back(std::move(_queued_tokens[i]));

  _attributes.push_back(std::move(attribute_declaration_));

  _all |= END_ATTRIBUTE_DECLARATION;
}

void file_builder::end_variable_declaration()
{
  variable_declaration variable_declaration_;
  variable_declaration_._variable_identifier = std::move(_queued_tokens[0]);

  variable_declaration_._values.reserve(_queued_tokens.size() - 1);

  for (std::size_t i = 1; i < _queued_tokens.size(); ++i)
    variable_declaration_._values.push_back(std::move(_queued_tokens[i]));

  _variables.push_back(std::move(variable_declaration_));

  _all |= END_VARIABLE_DECLARATION;
}

void file_builder::enqueue_token(token token_)
{
  _queued_tokens.push_back(std::move(token_));
}

void file_builder::clear_queue()
{
  _queued_tokens.clear();
}

auto file_builder::get_result() -> std::optional<std::variant<log_file, header_file>>
{
  switch (identify_filetype())
  {
  case LOG_FILE:
    return assemble_log_file();
  case HEADER_FILE:
    return assemble_header_file();
  case INVALID:
  default:
    return std::nullopt;
  }
}

auto file_builder::identify_filetype() -> std::uint8_t
{
  std::uint8_t const allowed_log_file    = END_DATE | END_ENTRY_IDENTIFIER | END_IVPL;
  std::uint8_t const allowed_header_file = END_ATTRIBUTE_DECLARATION | END_VARIABLE_DECLARATION;

  if (_all & allowed_log_file)
  {
    if (_all & allowed_header_file)
    {
      _warning_collector.add_warning(
        warning_code::MIXED_FILE,
        warning::location{ ._filepath = _filepath },
        "file contains both log and header file content");
      return INVALID;
    }
    return LOG_FILE;
  }

  if (_all & allowed_header_file)
  {
    if (_all & allowed_log_file)
    {
      _warning_collector.add_warning(
        warning_code::MIXED_FILE,
        warning::location{ ._filepath = _filepath },
        "file contains both log and header file content");
      return INVALID;
    }
    return HEADER_FILE;
  }

  if (_all == NONE)
  {
    _warning_collector.add_warning(
      warning_code::EMPTY_FILE,
      warning::location{ ._filepath = _filepath },
      "file is empty");
    return INVALID;
  }

  // Invalid in some other way
  _warning_collector.add_warning(
    warning_code::INVALID_FILE,
    warning::location{ ._filepath = _filepath },
    "file is invalid");

  return INVALID;
}

auto file_builder::assemble_log_file() -> log_file
{
  return log_file{ ._filepath = _filepath, ._date = std::move(*_date), ._entries = std::move(_entries) };
}

auto file_builder::assemble_header_file() -> header_file
{
  return header_file{ ._filepath   = _filepath,
                      ._attributes = std::move(_attributes),
                      ._variables  = std::move(_variables) };
}

} // namespace pmt