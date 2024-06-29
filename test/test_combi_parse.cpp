#include "test.hpp"

#include "logera/file_builder/file_builder.hpp"
#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/serial_warning_collector/serial_warning_collector.hpp"
#include "logera/warning/warning.hpp"
#include "logera/warning_code/warning_code.hpp"

#include "external/pegtl/pegtl.hpp"
#include "external/pegtl/pegtl/contrib/analyze.hpp"
#include "external/pegtl/pegtl/contrib/parse_tree.hpp"
#include "external/pegtl/pegtl/contrib/parse_tree_to_dot.hpp"

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace
{
using namespace tao::pegtl;
// clang-format off

// Charsets
struct charset_ws : one< ' ', '\t' > {};
struct charset_identifier_first : ranges< 'a', 'z', 'A', 'Z' > {};
struct charset_identifier_rest : sor< charset_identifier_first, digit, one< '_', '-', '.' > > {};

// Terminals
struct term_ws
  : plus< charset_ws > {};
struct term_eol
  : sor< string< '\r', '\n' >, one< '\n' > > {};
struct term_eol_comment :
  seq< one< '#' >, until< term_eol > > {};
struct term_semicolon :
  one< ';' > {};
struct term_colon :
  one< ':' > {};
struct term_attr_value :
  plus< not_one< ';', '#', ' ', '\t', '\n', '\r'> > {};
struct term_regex :
  term_attr_value {};
struct term_identifier :
  seq< charset_identifier_first, star< charset_identifier_rest > > {};
struct term_date :
  seq<rep<4, digit>, one< '-' >, rep<2, digit>, one< '-' >, rep<2, digit> > {};
struct term_kw_var :
  string< 'v', 'a', 'r', ':' > {};
struct term_kw_attr :
  string< 'a', 't', 't', 'r', ':' > {};

// Rules

struct rule_skip_line :
  until<eol, any> {};

struct rule_eol :
  sor<term_eol_comment, term_eol> {};

struct begin_entry; // Forward declaration
struct rule_entry_start :
  seq<at<term_identifier, term_colon>, apply<begin_entry>, term_identifier, term_colon>{};

struct rule_identifier_value_pair :
  seq<term_identifier, term_ws, term_attr_value> {};

struct rule_identifier_value_pair_list :
  seq<list<rule_identifier_value_pair, term_semicolon, term_ws>, term_semicolon> {};

struct rule_regex_list :
  star<seq<pad<term_regex, term_ws>, term_semicolon>> {};

struct rule_declare_attribute :
  seq<term_kw_attr, term_ws, term_identifier, opt<term_ws>, term_semicolon, rule_regex_list>{};

struct rule_identifier_list :
  star<seq<pad<term_identifier, term_ws>, term_semicolon>> {};

struct rule_declare_variable :
  seq<term_kw_var, term_ws, term_identifier, opt<term_ws>, term_semicolon, rule_identifier_list>{};

struct rule_line_core :
  sor<term_date, rule_declare_attribute, rule_declare_variable, rule_entry_start, rule_identifier_value_pair_list> {};

struct rule_line :
  seq<pad_opt<rule_line_core, term_ws>, rule_eol> {};

struct rule_line_or_skip :
  sor<rule_line, rule_skip_line> {};

struct rule_file :
  must<seq<star<rule_line_or_skip>, eof>> {};

// Actions
template <typename RULE>
struct my_action : nothing<RULE> {};

template<>
struct my_action<term_date>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.push_date(pmt::token{._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line});
  }
};

struct begin_entry
{

template <typename INPUT>
static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
{
  builder_.begin_entry();
}

};

template <>
struct my_action<term_kw_attr>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.begin_attribute_declaration();
  }
};

template <>
struct my_action<term_kw_var>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.begin_variable_declaration();
  }
};

template <>
struct my_action<rule_skip_line>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder&, pmt::i_warning_collector& warning_collector_)
  {
    warning_collector_.add_warning(pmt::warning_code::SKIPPING_LINE, pmt::warning::location{._filepath = in_.position().source, ._lineno = in_.position().line}, "skipping line");
  }
};

template <>
struct my_action<term_identifier>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.push_identifier(pmt::token{._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line});
  }
};

template <>
struct my_action<term_attr_value>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.push_attribute_value(pmt::token{._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line});
  }
};

template <>
struct my_action<term_regex>
{
  template <typename INPUT>
  static void apply(INPUT const& in_, pmt::file_builder& builder_, pmt::i_warning_collector&)
  {
    builder_.push_regex(pmt::token{._lexeme = in_.string(), ._colno = in_.position().column, ._lineno = in_.position().line});
  }
};

// clang-format on
} // namespace

// Parse function
template <typename RULE>
auto parse_input(std::string_view input_, pmt::file_builder& builder_, pmt::i_warning_collector& warning_collector_)
  -> bool
{
  memory_input in(input_, "filename.txt");
  return tao::pegtl::parse<RULE, my_action>(in, builder_, warning_collector_);
}

auto main(int argc, char** argv) -> int
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  // Read the file fully into a string
  std::ifstream input_file(argv[1]);
  if (!input_file.is_open())
  {
    std::cerr << "Failed to open file: " << argv[1] << '\n';
    return 1;
  }

  std::string input_string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

  pmt::serial_warning_collector warning_collector;
  pmt::file_builder             builder(warning_collector, argv[1]);

  // Parse the input
  auto const start = std::chrono::high_resolution_clock::now();
  auto       root  = parse_input<rule_file>(input_string, builder, warning_collector);
  auto const end   = std::chrono::high_resolution_clock::now();

  std::cout << "\n\nTime taken: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start) << "\n";

  std::cout << "Warnings:\n";
  std::vector<pmt::warning> warnings;
  warning_collector.release_warnings(std::back_inserter(warnings));

  for (auto const& warning : warnings)
  {
    std::cout << "\t- " << warning._msg << " at ";
    std::string filename = warning._location._filepath.has_value() ? warning._location._filepath.value().string() : "-";
    std::string lineno
      = warning._location._lineno.has_value() ? std::to_string(warning._location._lineno.value()) : "-";
    std::string colno = warning._location._colno.has_value() ? std::to_string(warning._location._colno.value()) : "-";

    std::cout << filename << ":" << lineno << ":" << colno << "\n";
  }

  if (root)
  {
    std::cout << "Parsing succeeded\n";
  }
  else
  {
    std::cerr << "Parsing failed\n";
    return 1;
  }

  return 0;
}