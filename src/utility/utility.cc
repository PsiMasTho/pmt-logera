//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "utility.h"
#include "../lexer/tokens.h"

#include <cstring> // memchr

using namespace std;

#if __unix__
#    include "read_file_unix-inl.h"
#else
#    include "read_file_portable-inl.h"
#endif

void erase_and_replace(string* target, string const& to_erase, string const& to_replace)
{
    size_t pos = target->find(to_erase);
    while(pos != string::npos)
    {
        target->erase(pos, to_erase.size());
        target->insert(pos, to_replace);
        pos = target->find(to_erase, pos + to_replace.size());
    }
}

auto count_line_nr(char const* buffer, u32 char_index) -> u32
{
    u32 ret = 1;
    char const* p = buffer;
    while(p)
    {
        p = (char*)memchr(p, '\n', char_index - (p - buffer));
        u32 const incr = p ? 1 : 0;
        ret += incr;
        p += incr;
    }
    return ret;
}

auto to_string(std::string_view sv) -> std::string
{
    return std::string(sv.data(), sv.size());
}

char const *token_names[]
{
    "DECL_VAR",
    "DECL_ATTR",
    "DATE",
    "ATTR_VALUE",
    "IDENT",
    "REGEX"
};

auto name_of_token(tok_t tok) -> std::string
{
    if(tok >= tokens::Tokens_::MAX_TOKEN)
        return "INVALID_TOKEN";

    if(tok >= 257)
        return token_names[tok - 257];

        // otherwise, it's a single character token
    return std::string(1, static_cast<char>(tok));
}

auto read_file(char const* filename) -> buffer_t
{
#if __unix__
    return read_file_unix(filename);
#else
    return read_file_portable(filename);
#endif
}