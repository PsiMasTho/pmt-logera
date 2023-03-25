//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_STRING_UTIL_H
#define INCLUDED_STRING_UTIL_H

#include <iosfwd>

void eraseAndReplace(std::string* target, std::string const& toErase, std::string const& toReplace);

#endif