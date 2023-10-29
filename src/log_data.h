//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "date.h"
#include "sparse_array.h"

#include <string>
#include <vector>

// class representing data of a single file
class log_data
{
public:
    // log_line.get(0) == var name
    using log_line = sparse_array<std::string>;

private:
    friend class log_data_modifier;

    date m_date;
    std::vector<log_line> m_lines;

public:
    log_data() = default;
    date const& get_date() const;
    std::vector<log_line> const& get_lines() const;
};
