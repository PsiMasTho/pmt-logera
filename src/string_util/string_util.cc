//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "string_util.h"

#include <string>

using namespace std;

void eraseAndReplace(string* target, string const& toErase, string const& toReplace)
{
    size_t const pos = target->find(toErase);
    if(pos != string::npos)
    {
        target->erase(pos, toErase.size());
        target->insert(pos, toReplace);
    }
}