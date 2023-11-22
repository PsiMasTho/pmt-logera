#pragma once

#include <string>

struct header_tokens
{
    enum Tokens_
    {
        DECL_VAR = 257,
        DECL_ATTR,
        IDENT,
        NEWLINE,
        REGEX,

        MAX_TOKEN
    };

        // Note: This function is mainly used for testing the lexer.
    static auto get_name(Tokens_ token) -> std::string
    {
        static char const* const symbolic_tokens[] = 
        {
            "DECL_VAR",
            "DECL_ATTR",
            "IDENT",
            "NEWLINE",
            "REGEX",
        };

        if (token >= MAX_TOKEN)
            return "INVALID_TOKEN";

        if (token >= 257)
            return symbolic_tokens[token - 257];

        return std::string(1, static_cast<char>(token));
    }

    static auto get_name(int token) -> std::string
    {
        return get_name(static_cast<Tokens_>(token));
    }
};