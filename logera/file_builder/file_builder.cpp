#include "file_builder.hpp"

#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/warning/warning.hpp"
#include "logera/warning_code/warning_code.hpp"

#include <variant>

namespace pmt
{

namespace
{

enum history : std::uint8_t
{
  NONE                        = 0,
  BEGIN_ENTRY                 = 1 << 0,
  BEGIN_ATTRIBUTE_DECLARATION = 1 << 1,
  BEGIN_VARIABLE_DECLARATION  = 1 << 2,
  PUSH_DATE                   = 1 << 3,
  PUSH_IDENTIFIER             = 1 << 4,
  PUSH_REGEX                  = 1 << 5,
  PUSH_ATTRIBUTE_VALUE        = 1 << 6
};

enum filetype : std::uint8_t
{
  UNDECIDED   = 0,
  INVALID     = 1,
  LOG_FILE    = 2,
  HEADER_FILE = 3
};

} // namespace

file_builder::file_builder(i_warning_collector& warning_collector_, std::filesystem::path filepath_)
  : _state(NONE)
  , _previous(NONE)
  , _all(NONE)
  , _filetype(UNDECIDED)
  , _warning_collector(warning_collector_)
  , _filepath(std::move(filepath_))
{
}

void file_builder::begin_entry()
{
  if (_all & (BEGIN_ATTRIBUTE_DECLARATION | BEGIN_VARIABLE_DECLARATION))
    _filetype = INVALID;

  do_begin_entry();
  _filetype = _filetype == UNDECIDED ? LOG_FILE : INVALID;
  _state    = BEGIN_ENTRY;
  _previous = BEGIN_ENTRY;
  _all |= BEGIN_ENTRY;
}

void file_builder::begin_attribute_declaration()
{
  if (_all & (BEGIN_ENTRY | PUSH_DATE))
    _filetype = INVALID;

  do_begin_attribute_declaration();
  _filetype = _filetype == UNDECIDED ? HEADER_FILE : INVALID;
  _state    = BEGIN_ATTRIBUTE_DECLARATION;
  _previous = BEGIN_ATTRIBUTE_DECLARATION;
  _all |= BEGIN_ATTRIBUTE_DECLARATION;
}

void file_builder::begin_variable_declaration()
{
  if (_all & (BEGIN_ENTRY | PUSH_DATE))
    _filetype = INVALID;

  do_begin_variable_declaration();
  _filetype = _filetype == UNDECIDED ? HEADER_FILE : INVALID;
  _state    = BEGIN_VARIABLE_DECLARATION;
  _previous = BEGIN_VARIABLE_DECLARATION;
  _all |= BEGIN_VARIABLE_DECLARATION;
}

void file_builder::push_date(token date_)
{
  if (_all != NONE)
    _warning_collector.add_warning(
      warning_code::DATE_NOT_FIRST_IN_FILE,
      warning::location{ ._filepath = _filepath, ._lineno = date_._lineno, ._colno = date_._colno },
      "date must be the first token in the file");

  if (_all & (BEGIN_ATTRIBUTE_DECLARATION | BEGIN_VARIABLE_DECLARATION))
    _filetype = INVALID;

  do_push_date(date_);
  _filetype = _filetype == UNDECIDED ? LOG_FILE : INVALID;
  _state    = NONE;
  _previous = PUSH_DATE;
  _all |= PUSH_DATE;
}

void file_builder::push_identifier(token identifier_)
{
  do_push_identifier(identifier_);
  _previous = PUSH_IDENTIFIER;
  _all |= PUSH_IDENTIFIER;
}

void file_builder::push_regex(token regex_)
{
  do_push_regex(regex_);
  _previous = PUSH_REGEX;
  _all |= PUSH_REGEX;
}

void file_builder::push_attribute_value(token value_)
{
  do_push_attribute_value(value_);
  _previous = PUSH_ATTRIBUTE_VALUE;
  _all |= PUSH_ATTRIBUTE_VALUE;
}

auto file_builder::get_result() -> std::optional<std::variant<log_file, header_file>>
{
  switch (_filetype)
  {
  case LOG_FILE:
    return assemble_log_file();
  case HEADER_FILE:
    return assemble_header_file();
  case UNDECIDED:
  case INVALID:
  default:
    return std::nullopt;
  }
}

void file_builder::do_begin_entry()
{
}

void file_builder::do_begin_attribute_declaration()
{
}

void file_builder::do_begin_variable_declaration()
{
}

void file_builder::do_push_date(token& date_)
{
  if (_date.has_value())
    _warning_collector.add_warning(
      warning_code::REDEFINING_DATE,
      warning::location{ ._filepath = _filepath, ._lineno = date_._lineno, ._colno = date_._colno },
      "redefining date");

  _date = std::move(date_);
}

void file_builder::do_push_identifier(token& identifier_)
{
  switch (_state)
  {
  case BEGIN_ENTRY:
    switch (_previous)
    {
    case BEGIN_ENTRY:
      _entries.emplace_back(std::move(identifier_));
      break;
    case PUSH_IDENTIFIER:
    case PUSH_ATTRIBUTE_VALUE:
      _entries.back()._pairs.emplace_back(std::move(identifier_));
      break;
    default:
      std::terminate();
    }
    break;
  case BEGIN_ATTRIBUTE_DECLARATION:
    switch (_previous)
    {
    case BEGIN_ATTRIBUTE_DECLARATION:
      _attributes.emplace_back(std::move(identifier_));
      break;
    default:
      std::terminate();
    }
    break;
  case BEGIN_VARIABLE_DECLARATION:
    switch (_previous)
    {
    case BEGIN_VARIABLE_DECLARATION:
      _variables.emplace_back(std::move(identifier_));
      break;
    case PUSH_IDENTIFIER:
      _variables.back()._values.emplace_back(std::move(identifier_));
      break;
    default:
      std::terminate();
    }
    break;
  }
}

void file_builder::do_push_regex(token& regex_)
{
  switch (_state)
  {
  case BEGIN_ATTRIBUTE_DECLARATION:
    switch (_previous)
    {
    case PUSH_IDENTIFIER:
    case PUSH_REGEX:
      _attributes.back()._values.emplace_back(std::move(regex_));
      break;
    default:
      std::terminate();
    }
    break;
  default:
    std::terminate();
  }
}

void file_builder::do_push_attribute_value(token& value_)
{
  switch (_state)
  {
  case BEGIN_ENTRY:
    switch (_previous)
    {
    case PUSH_IDENTIFIER:
      _entries.back()._pairs.back()._value = std::move(value_);
      break;
    default:
      std::terminate();
    }
    break;
  default:
    std::terminate();
  }
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