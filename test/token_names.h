#pragma once

#include "../src/lexer/tokens.h"

inline auto get_name(log_tokens::Tokens_ token) -> std::string
{
    static char const* const symbolic_tokens[] = {"IDENT", "NEWLINE", "DATE", "IDENT_VALUE_PAIR"};

    if(token >= log_tokens::MAX_TOKEN)
        return "INVALID_TOKEN";

    if(token >= 257)
        return symbolic_tokens[token - 257];

    return std::string(1, static_cast<char>(token));
}

inline auto get_name(header_tokens::Tokens_ token) -> std::string
{
    static char const* const symbolic_tokens[] = {
        "DECL_VAR",
        "DECL_ATTR",
        "IDENT",
        "NEWLINE",
        "REGEX",
    };

    if(token >= header_tokens::Tokens_::MAX_TOKEN)
        return "INVALID_TOKEN";

    if(token >= 257)
        return symbolic_tokens[token - 257];

    return std::string(1, static_cast<char>(token));
}