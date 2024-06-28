#include "file_parse_task.hpp"
#if 0
#include "logera/combi_lex/combi_lex.hpp"
#include "logera/combi_parse/combi_parse.hpp"
#include "logera/i_file_reader/i_file_reader.hpp"
#include "logera/i_warning_collector/i_warning_collector.hpp"
#include "logera/overloaded/overloaded.hpp"
#include "logera/warning_code/warning_code.hpp"

namespace pmt
{

namespace
{
using lex = combi_lex;

// Charsets
using charset_ws     = lex::match_in_group<" \t">;
using charset_digit  = lex::match_in_range<'0', '9'>;
using charset_alpha  = lex::one_of<lex::match_in_range<'a', 'z'>, lex::match_in_range<'A', 'Z'>>;
using charset_alnum  = lex::one_of<charset_alpha, charset_digit>;
using charset_alphau = lex::one_of<charset_alpha, lex::match_char<'_'>>;
using charset_alnumu = lex::one_of<charset_alnum, lex::match_char<'_'>>;

// Terminals
using term_ws = lex::one_or_more<charset_ws>;

using term_eol = lex::one_of<lex::match_literal<"\r\n">, lex::match_char<'\n'>>;

using term_eol_comment
  = lex::sequence<lex::match_char<'#'>, lex::zero_or_more<lex::match_not_in_group<"\r\n">>, term_eol>;

using term_attr_value = lex::one_or_more<lex::match_not_in_group<";# \t">>;

using term_regex = term_attr_value;

using term_identifier = lex::sequence<charset_alpha, lex::zero_or_more<charset_alnumu>>;

using term_date = lex::sequence<
  lex::exactly_n<charset_digit, 4>,
  lex::match_char<'-'>,
  lex::exactly_n<charset_digit, 2>,
  lex::match_char<'-'>,
  lex::exactly_n<charset_digit, 2>>;

struct parsing_context
{
  i_warning_collector& _warning_collector;
  std::string_view     _buffer;

  auto capture_state() -> std::string_view
  {
    return _buffer;
  }

  void restore_state(std::string_view state_)
  {
    _buffer = state_;
  }
};

template <typename T>
struct lex_with_token
{
  using return_type = token;
  auto operator()(parsing_context& ctx_) -> std::optional<return_type>
  {
    lex::context lex_ctx(ctx_._buffer);

    if (T{}(lex_ctx))
    {
      ctx_._buffer = lex_ctx.get_remaining();

      std::string_view const match{ lex_ctx.get_match() };
      return token{
        ._lexeme = std::string{match.begin(), match.end()},
        ._colno  = 0
      };
    }

    return std::nullopt;
  }
};

template <typename T>
struct lex_without_token
{
  struct return_type
  {
  };
  auto operator()(parsing_context& ctx_) -> std::optional<return_type>
  {
    lex::context lex_ctx(ctx_._buffer);

    if (T{}(lex_ctx))
    {
      ctx_._buffer = lex_ctx.get_remaining();
      return return_type{};
    }
    return std::nullopt;
  }
};

struct on_error
{
  using return_type = invalid_line;
  auto operator()(parsing_context&) -> std::optional<return_type>
  {
    /*
    ctx_._warning_collector.add_warning(
      warning_code::SKIPPING_LINE,
      warning::location{ ctx_._filepath, ctx_._lineno },
      "Skipping line");
      */
    return return_type{};
  }
};

// Rules
using prs = combi_parse<parsing_context>;

using parse_ws = lex_without_token<term_ws>;

using parse_comment = lex_with_token<term_eol_comment>;

struct parse_blank_line
  : prs::converter<parse_blank_line, blank_line, prs::sequence<prs::maybe<parse_ws>, prs::maybe<parse_comment>>>
{
  static auto convert(input_type&, parsing_context&) -> return_type
  {
    return blank_line{};
  }
};

struct parse_date_line : prs::converter<
                           parse_date_line,
                           date_line,
                           prs::sequence<lex_with_token<term_date>, parse_ws, prs::maybe<parse_comment>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    date_line ret;
    ret._date = std::move(get<0>(item_));
    return ret;
  }
};

struct parse_entry_start_line : prs::converter<
                                  parse_entry_start_line,
                                  entry_start_line,
                                  prs::sequence<
                                    lex_with_token<term_identifier>,
                                    lex_without_token<lex::match_char<':'>>,
                                    parse_ws,
                                    prs::maybe<parse_comment>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    entry_start_line ret;
    ret._entry_identifier = std::move(get<0>(item_));
    return ret;
  }
};

struct parse_identifier_value_pair : prs::converter<
                                       parse_identifier_value_pair,
                                       identifier_value_pair,
                                       prs::sequence<
                                         parse_ws,
                                         lex_with_token<term_identifier>,
                                         parse_ws,
                                         lex_with_token<term_attr_value>,
                                         parse_ws,
                                         lex_without_token<lex::match_char<';'>>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    identifier_value_pair ret;
    ret._identifier = std::move(get<1>(item_));
    ret._value      = std::move(get<3>(item_));
    return ret;
  }
};

struct parse_identifier_value_pair_list
  : prs::converter<
      parse_identifier_value_pair_list,
      identifier_value_pair_list_line,
      prs::sequence<prs::one_or_more<parse_identifier_value_pair>, parse_ws, prs::maybe<parse_comment>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    identifier_value_pair_list_line ret;
    ret._pairs = std::move(get<0>(item_));
    return ret;
  }
};

struct parse_regex_list
  : prs::converter<
      parse_regex_list,
      std::vector<token>,
      prs::zero_or_more<
        prs::sequence<parse_ws, lex_with_token<term_regex>, parse_ws, lex_without_token<lex::match_char<';'>>>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    std::vector<token> ret;
    ret.reserve(item_.size());
    for (auto& sequence : item_)
      ret.push_back(std::move(get<1>(sequence)));
    return ret;
  }
};

struct parse_declare_attribute_line : prs::converter<
                                        parse_declare_attribute_line,
                                        declare_attribute_line,
                                        prs::sequence<
                                          lex_without_token<lex::match_literal<"attr:">>,
                                          parse_ws,
                                          lex_with_token<term_identifier>,
                                          parse_ws,
                                          lex_without_token<lex::match_char<';'>>,
                                          parse_regex_list,
                                          parse_ws,
                                          prs::maybe<parse_comment>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    declare_attribute_line ret;
    ret._attribute_identifier = std::move(get<2>(item_));
    ret._values               = std::move(get<5>(item_));
    return ret;
  }
};

struct parse_identifier_list
  : prs::converter<
      parse_identifier_list,
      std::vector<token>,
      prs::zero_or_more<
        prs::sequence<parse_ws, lex_with_token<term_identifier>, parse_ws, lex_without_token<lex::match_char<';'>>>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    std::vector<token> ret;
    ret.reserve(item_.size());
    for (auto& sequence : item_)
      ret.push_back(std::move(get<1>(sequence)));
    return ret;
  }
};

struct parse_declare_variable_line : prs::converter<
                                       parse_declare_variable_line,
                                       declare_variable_line,
                                       prs::sequence<
                                         lex_without_token<lex::match_literal<"var:">>,
                                         parse_ws,
                                         lex_with_token<term_identifier>,
                                         parse_ws,
                                         lex_without_token<lex::match_char<';'>>,
                                         parse_identifier_list,
                                         parse_ws,
                                         prs::maybe<parse_comment>>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    declare_variable_line ret;
    ret._variable_identifier = std::move(get<2>(item_));
    ret._values              = std::move(get<5>(item_));
    return ret;
  }
};

struct parse_line : prs::converter<
                      parse_line,
                      line,
                      prs::one_of<
                        parse_blank_line,
                        parse_date_line,
                        parse_entry_start_line,
                        parse_identifier_value_pair_list,
                        parse_declare_attribute_line,
                        parse_declare_variable_line,
                        on_error>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    return visit(overloaded{ [](auto& x_) -> line { return std::move(x_); } }, item_);
  }
};

struct parse_file : prs::converter<parse_file, file, prs::zero_or_more<parse_line>>
{
  static auto convert(input_type& item_, parsing_context&) -> return_type
  {
    file ret;
    ret._lines = std::move(item_);
    return ret;
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

auto file_parse_task::get_result() -> std::optional<file>&
{
  return _result;
}

void file_parse_task::execute_impl()
{
  std::string_view buffer = _file_reader.read(_filepath.c_str());

  parsing_context ctx{ _warning_collector, buffer };

  _result = parse_file{}(ctx);

  if (_result.has_value())
    _result->_filepath = _filepath;
}

} // namespace pmt

#endif