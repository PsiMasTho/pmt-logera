#pragma once

#include <string>
#include <cstdint>

using underlying_token_t = int;

struct log_tokens
{
    enum Tokens_ : underlying_token_t
    {
        IDENT = 257,
        NEWLINE,
        DATE,
        IDENT_VALUE_PAIR,

        MAX_TOKEN
    };
};

struct header_tokens
{
    enum Tokens_ : underlying_token_t
    {
        DECL_VAR = 257,
        DECL_ATTR,
        IDENT,
        NEWLINE,
        REGEX,

        MAX_TOKEN
    };
};

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