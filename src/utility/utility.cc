//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "utility.h"

#include <cstring>   // memchr

using namespace std;

#if __unix__
    #include "read_file_unix-inl.h"
#else
    #include "read_file_portable-inl.h"
#endif

void erase_and_replace(string* target, string const& to_erase, string const& to_replace)
{
    size_t pos = target->find(to_erase);
    while (pos != string::npos)
    {
        target->erase(pos, to_erase.size());
        target->insert(pos, to_replace);
        pos = target->find(to_erase, pos + to_replace.size());
    }
}

auto read_file(char const* filename, uint32_t* size) -> unique_ptr<char[]>
{
    #if __unix__
        return read_file_unix(filename, size);
    #else
        return read_file_portable(filename, size);
    #endif
}

auto count_line_nr(char const* buffer, uint32_t char_index) -> uint32_t
{
    uint32_t ret = 1;
    char const* p = buffer;
    while (p)
    {
        p = (char*) memchr(p, '\n', char_index - (p - buffer));
        uint32_t const incr = p ? 1 : 0;
        ret += incr;
        p += incr;
    }
    return ret;
}