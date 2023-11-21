//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "utility.h"

#include <string>

using namespace std;

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