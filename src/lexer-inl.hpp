// clang-format off
#ifdef __INTELLISENSE__
    #include "lexer.hpp"
#endif
// clang-format on

template <> inline auto lexer::lex_token<token::COLON>(std::string_view* dest) -> bool
{
    return match_literal(":", dest);
}

template <> inline auto lexer::lex_token<token::SEMICOLON>(std::string_view* dest) -> bool
{
    return match_literal(";", dest);
}

template <> inline auto lexer::lex_token<token::KW_ATTR>(std::string_view* dest) -> bool
{
    return match_literal("attr", dest);
}

template <> inline auto lexer::lex_token<token::KW_VAR>(std::string_view* dest) -> bool
{
    return match_literal("var", dest);
}

template <> inline auto lexer::lex_token<token::IDENT>(std::string_view* dest) -> bool
{
    std::cmatch match;
    if (!std::regex_search(m_cursor, m_buffer + m_buffer_size, match, m_regexes[token::IDENT], std::regex_constants::match_continuous))
        return false;

    if (match.str() == "attr" || match.str() == "var")
        return false;

    m_cursor += match.length();

    if (dest)
        *dest = std::string_view{ match[0].first, static_cast<size_t>(match.length()) };
    return true;
}

template <> inline auto lexer::lex_token<lexer::SKIPWS>(std::string_view*) -> bool
{
    skipws();
    return true;
}

template <int tok> auto lexer::lex_token(std::string_view* dest) -> bool
{
    std::cmatch match;
    if (!std::regex_search(m_cursor, m_buffer + m_buffer_size, match, m_regexes[tok], std::regex_constants::match_continuous))
        return false;

    m_cursor += match.length();

    if (dest)
        *dest = std::string_view{ match[0].first, static_cast<size_t>(match.length()) };
    return true;
}

template <int... toks> auto lexer::lex_tokens(typename meta::repeat_tuple<sizeof...(toks), std::string_view*>::type dests) -> bool
{
    return [&]<std::size_t... indices>(std::index_sequence<indices...>) -> bool
    { return (lex_token<toks>(std::get<indices>(dests)) && ...); }(std::make_index_sequence<sizeof...(toks)>{});
}
