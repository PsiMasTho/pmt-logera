#include "file_parse_task.hpp"

#include "external/pegtl/pegtl/rules.hpp"
#include "logera/file_builder/file_builder.hpp"
#include "logera/i_file_reader/i_file_reader.hpp"
#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/warning_code/warning_code.hpp"

#include "external/pegtl/pegtl.hpp"

namespace pmt
{

namespace
{
using namespace tao::pegtl;
// clang-format off

// Actions
struct end_date
{
  static void apply0(pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.end_date();
  }
};

struct end_entry_identifier
{
  static void apply0(pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.end_entry_identifier();
  }
};

struct end_ivpl
{
  static void apply0(pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.end_ivpl();
  }
};

struct end_attribute_declaration
{
  static void apply0(pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.end_attribute_declaration();
  }
};

struct end_variable_declaration
{
  static void apply0(pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.end_variable_declaration();
  }
};

struct clear_queue
{
  static void apply0(pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.clear_queue();
  }
};

template <typename RULE>
struct action_for_every : nothing<RULE>
{
};

// Charsets
struct charset_ws : one< ' ', '\t' > {};
struct charset_identifier_first : ranges< 'a', 'z', 'A', 'Z' > {};
struct charset_identifier_rest : sor< charset_identifier_first, digit, one< '_', '-', '.' > > {};

// Terminals
struct term_ws
  : plus< charset_ws > {};
struct term_eol :
  sor<eol, seq<eof, failure>> {};
struct term_eol_comment :
  seq< one< '#' >, until<term_eol>> {};
struct term_semicolon :
  one< ';' > {};
struct term_colon :
  one< ':' > {};
struct term_attr_value :
  plus< not_one< ';', '#', ' ', '\t', '\n', '\r'> > {};
struct term_regex :
  term_attr_value {};
struct term_identifier :
  minus<seq<charset_identifier_first, star<charset_identifier_rest>>, sor<string<'v', 'a', 'r'>, string<'a', 't', 't', 'r'>>> {};
struct term_date :
  seq<rep<4, digit>, one<'-'>, rep<2, digit>, one<'-'>, rep<2, digit> > {};
struct term_kw_var :
  string<'v', 'a', 'r', ':'> {};
struct term_kw_attr :
  string<'a', 't', 't', 'r', ':'> {};

// Rules

struct rule_eol :
  sor<term_eol, term_eol_comment> {};

struct rule_skip_line :
  until<rule_eol, any> {};

struct rule_date :
  seq<term_date, apply0<end_date>> {};

struct rule_entry_start :
  seq<term_identifier, term_colon, apply0<end_entry_identifier>>{};

struct rule_identifier_value_pair :
  seq<term_identifier, term_ws, term_attr_value> {};

struct rule_identifier_value_pair_list :
  seq<list<rule_identifier_value_pair, term_semicolon, term_ws>, term_semicolon, apply0<end_ivpl>> {};

struct rule_regex_list :
  star<seq<pad<term_regex, term_ws>, term_semicolon>> {};

struct rule_declare_attribute :
  seq<term_kw_attr, term_ws, term_identifier, opt<term_ws>, term_semicolon, rule_regex_list, apply0<end_attribute_declaration>>{};

struct rule_identifier_list :
  star<seq<pad<term_identifier, term_ws>, term_semicolon>> {};

struct rule_declare_variable :
  seq<term_kw_var, term_ws, term_identifier, opt<term_ws>, term_semicolon, rule_identifier_list, apply0<end_variable_declaration>>{};

struct rule_line_core :
  sor<
    seq<apply0<clear_queue>, rule_date>,
    seq<apply0<clear_queue>, rule_declare_attribute>,
    seq<apply0<clear_queue>, rule_declare_variable>,
    seq<apply0<clear_queue>, rule_entry_start>,
    seq<apply0<clear_queue>, rule_identifier_value_pair_list>
  > {};

struct rule_line :
  seq<pad_opt<rule_line_core, term_ws>, rule_eol> {};

struct rule_line_or_skip :
  sor<rule_line, rule_skip_line> {};

struct rule_file :
  must<star<rule_line_or_skip>> {};
// clang-format on

// Actions

template <>
struct action_for_every<rule_skip_line>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector& warning_collector_)
  {
    warning_collector_.add_warning(
      pmt::warning_code::SKIPPING_LINE,
      pmt::warning::location{ ._filepath = in_.position().source, ._lineno = in_.position().line },
      "skipping line");

    builder_.clear_queue();
  }
};

template <>
struct action_for_every<term_date>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.enqueue_token(
      pmt::token{ ._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line });
  }
};

template <>
struct action_for_every<term_identifier>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.enqueue_token(
      pmt::token{ ._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line });
  }
};

template <>
struct action_for_every<term_attr_value>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.enqueue_token(
      pmt::token{ ._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line });
  }
};

template <>
struct action_for_every<term_regex>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.enqueue_token(
      pmt::token{ ._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line });
  }
};

} // namespace

file_parse_task::file_parse_task(
  i_file_reader&        file_reader_,
  i_warning_collector&  warning_collector_,
  std::filesystem::path filepath_)
  : _file_reader(file_reader_)
  , _warning_collector(warning_collector_)
  , _filepath(std::move(filepath_))
  , _result{ std::nullopt }
{
}

auto file_parse_task::get_result() -> std::optional<std::variant<log_file, header_file>>&
{
  return _result;
}

void file_parse_task::execute_impl()
{
  std::string_view  buffer = _file_reader.read(_filepath.c_str());
  pmt::file_builder builder(_warning_collector, _filepath);

  memory_input in(buffer, _filepath);
  tao::pegtl::parse<rule_file, action_for_every>(in, builder, _warning_collector);

  _result = builder.get_result();
}

} // namespace pmt
