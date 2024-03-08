#include "logera/parser.hpp"

#include "logera/ast.hpp"
#include "logera/errors.hpp"
#include "logera/lexer.hpp"
#include "logera/overloaded.hpp"
#include "logera/tokens.hpp"

#include <cassert>

using namespace std;

// clang-format off
/* EBNF:

<file> ::= <line>*

<line>                  ::= <NEWLINE>                         |
                            <DATE> <NEWLINE>                  |
                            <declare_attribute> <NEWLINE>     |
                            <declare_variable> <NEWLINE>      |
                            <IDENT> <COLON> <NEWLINE>         |
                            <ident_value_pair_list> <NEWLINE>

<declare_attribute>     ::= <KW_ATTR> <COLON> <IDENT> <SEMICOLON> (<REGEX> <SEMICOLON>)*

<declare_variable>      ::= <KW_VAR> <COLON> <IDENT> <SEMICOLON> (<IDENT> <SEMICOLON>)*

<ident_value_pair_list> ::= (<ident_value_pair> <SEMICOLON>)+

<ident_value_pair>      ::= <IDENT> <ATTR_VALUE>

*/
// clang-format on

parser::parser(lexer& lexer, flyweight_string::storage_type& storage, error::container& errors)
    : m_lexer(lexer)
    , m_storage(storage)
    , m_errors(errors)
{
}

auto parser::parser::parse() -> ast::file_node
{
    ast::file_node root{ .filename = m_lexer.get_filename(), .children = {} };

    parsed_line line;
    while (parse_line(&line))
    {
        visit(
            overloaded{ [&](auto&& n) { root.children.push_back(std::move(n)); }, [&](std::monostate&&) {} },
            std::move(line));
    }

    return root;
}

auto parser::parser::parse_line(parsed_line* dest) -> parsed_line*
{
    char const *furthest = nullptr, *initial = m_lexer.get_cursor();
    string_view tok;
    auto const  failure_rewind = [&]
    {
        furthest = max(furthest, m_lexer.get_cursor());
        m_lexer.set_cursor(initial);
    };

    // <NEWLINE>
    if (m_lexer.lex_tokens<lexer::SKIPWS, token::NEWLINE>(make_tuple(nullptr, nullptr)))
    {
        *dest = std::monostate{};
        return dest; // success
    }
    failure_rewind();

    // <DATE> <NEWLINE>
    if (m_lexer.lex_tokens<lexer::SKIPWS, token::DATE, lexer::SKIPWS, token::NEWLINE>(
            make_tuple(nullptr, &tok, nullptr, nullptr)))
    {
        *dest = ast::date_node{
            .record = token::record{.location = m_lexer.get_location(tok.data()),
                                    .lexeme   = flyweight_string{ tok, m_storage },
                                    .token    = token::DATE}
        };
        return dest; // success
    }
    failure_rewind();

    // <declare_attribute> <NEWLINE>
    if (ast::decl_attr_node ret; parse_declare_attribute(&ret))
    {
        if (m_lexer.lex_tokens<lexer::SKIPWS, token::NEWLINE>(make_tuple(nullptr, nullptr)))
        {
            *dest = std::move(ret);
            return dest; // success
        }
    }
    failure_rewind();

    // <declare_variable> <NEWLINE>
    if (ast::decl_var_node ret; parse_declare_variable(&ret))
    {
        if (m_lexer.lex_tokens<lexer::SKIPWS, token::NEWLINE>(make_tuple(nullptr, nullptr)))
        {
            *dest = std::move(ret);
            return dest; // success
        }
    }
    failure_rewind();

    // <IDENT> <COLON> <NEWLINE>
    if (m_lexer.lex_tokens<lexer::SKIPWS, token::IDENT, token::COLON, lexer::SKIPWS, token::NEWLINE>(
            make_tuple(nullptr, &tok, nullptr, nullptr, nullptr)))
    {
        *dest = ast::identifier_node{
            .record = token::record{.location = m_lexer.get_location(tok.data()),
                                    .lexeme   = flyweight_string{ tok, m_storage },
                                    .token    = token::IDENT}
        };
        return dest; // success
    }
    failure_rewind();

    // <ident_value_pair_list>
    if (ast::ident_value_pair_list_node ret; parse_ident_value_pair_list(&ret))
    {
        if (m_lexer.lex_tokens<lexer::SKIPWS, token::NEWLINE>(make_tuple(nullptr, nullptr)))
        {
            *dest = std::move(ret);
            return dest; // success
        }
    }
    failure_rewind();

    // end of file
    if (m_lexer.get_remaining() == 0)
        return nullptr;

    // error
    m_errors.emplace_back(error::make_record<error::unexpected_token>(m_lexer.get_location(furthest)));
    *dest = std::monostate{};
    m_lexer.skipln();
    return dest;
}

auto parser::parser::parse_declare_attribute(ast::decl_attr_node* dest) -> ast::decl_attr_node*
{
    assert(dest != NULL);
    string_view tok;

    // <KW_ATTR> <COLON> <IDENT> <SEMICOLON>
    if (!m_lexer.lex_tokens<
            lexer::SKIPWS,
            token::KW_ATTR,
            token::COLON,
            lexer::SKIPWS,
            token::IDENT,
            lexer::SKIPWS,
            token::SEMICOLON>(make_tuple(nullptr, nullptr, nullptr, nullptr, &tok, nullptr, nullptr)))
        return nullptr;

    dest->identifier = ast::identifier_node{
        .record = token::record{.location = m_lexer.get_location(tok.data()),
                                .lexeme   = flyweight_string{ tok, m_storage },
                                .token    = token::IDENT}
    };

    // (<REGEX> <SEMICOLON>)*
    while (1)
    {
        if (!m_lexer.lex_tokens<lexer::SKIPWS, token::REGEX, lexer::SKIPWS, token::SEMICOLON>(
                make_tuple(nullptr, &tok, nullptr, nullptr)))
            break;

        dest->children.push_back(ast::regex_node{
            .record = token::record{.location = m_lexer.get_location(tok.data()),
                                    .lexeme   = flyweight_string{ tok, m_storage },
                                    .token    = token::REGEX}
        });
    }

    return dest;
}

auto parser::parse_declare_variable(ast::decl_var_node* dest) -> ast::decl_var_node*
{
    assert(dest != NULL);
    string_view tok;

    // <KW_VAR> <COLON> <IDENT> <SEMICOLON>
    if (!m_lexer.lex_tokens<
            lexer::SKIPWS,
            token::KW_VAR,
            token::COLON,
            lexer::SKIPWS,
            token::IDENT,
            lexer::SKIPWS,
            token::SEMICOLON>(make_tuple(nullptr, nullptr, nullptr, nullptr, &tok, nullptr, nullptr)))
        return nullptr;

    dest->identifier = ast::identifier_node{
        .record = token::record{.location = m_lexer.get_location(tok.data()),
                                .lexeme   = flyweight_string{ tok, m_storage },
                                .token    = token::IDENT}
    };

    // (<IDENT> <SEMICOLON>)*
    while (1)
    {
        if (!m_lexer.lex_tokens<lexer::SKIPWS, token::IDENT, lexer::SKIPWS, token::SEMICOLON>(
                make_tuple(nullptr, &tok, nullptr, nullptr)))
            break;

        dest->children.push_back(ast::identifier_node{
            .record = token::record{.location = m_lexer.get_location(tok.data()),
                                    .lexeme   = flyweight_string{ tok, m_storage },
                                    .token    = token::IDENT}
        });
    }

    return dest;
}

auto parser::parse_ident_value_pair_list(ast::ident_value_pair_list_node* dest) -> ast::ident_value_pair_list_node*
{
    assert(dest != NULL);

    ast::ident_value_pair_list_node tmp;

    int                        n_pairs = 0;
    ast::ident_value_pair_node ivp;
    while (1)
    {
        char const* initial = m_lexer.get_cursor();
        if (!parse_ident_value_pair(&ivp))
        {
            m_lexer.set_cursor(initial);
            break;
        }
        tmp.children.push_back(ivp);

        if (!m_lexer.lex_tokens<lexer::SKIPWS, token::SEMICOLON>(make_tuple(nullptr, nullptr)))
        {
            m_lexer.set_cursor(initial);
            break;
        }

        ++n_pairs;
    }

    // (<ident_value_pair> <SEMICOLON>)+
    if (n_pairs == 0)
        return nullptr;

    *dest = std::move(tmp);
    return dest;
}

auto parser::parse_ident_value_pair(ast::ident_value_pair_node* dest) -> ast::ident_value_pair_node*
{
    assert(dest != NULL);
    string_view tok_ident, tok_val;

    // <IDENT> <ATTR_VALUE>
    if (!m_lexer.lex_tokens<lexer::SKIPWS, token::IDENT, lexer::SKIPWS, token::ATTR_VALUE>(
            make_tuple(nullptr, &tok_ident, nullptr, &tok_val)))
        return nullptr;

    dest->identifier = ast::identifier_node{
        .record = token::record{.location = m_lexer.get_location(tok_ident.data()),
                                .lexeme   = flyweight_string{ tok_ident, m_storage },
                                .token    = token::IDENT}
    };

    dest->attr_value = ast::attr_value_node{
        .record = token::record{.location = m_lexer.get_location(tok_val.data()),
                                .lexeme   = flyweight_string{ tok_val, m_storage },
                                .token    = token::ATTR_VALUE}
    };

    return dest;
}