//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <stdexcept>
#include <string>

struct cmdl_exception : public std::invalid_argument
{
    explicit cmdl_exception(std::string const& what) : std::invalid_argument(what) {}
};