//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "utility.h"

#include <cstring> // memchr

using namespace std;



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