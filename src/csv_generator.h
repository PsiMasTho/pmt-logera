//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "archive_data.h"
#include <filesystem>
#include <iosfwd>
#include <string>

class entry;

// Writes log lines or vectors of strings to a
// file in CSV format according to RFC 4180.
class csv_generator
{
    std::ostream& m_out;
    char m_delim;

public:
    csv_generator(std::ostream& out);
    void write(log_date const& log_date, std::string const& filename, entry_data const& entry);
    void write(std::vector<std::string> const& vec);
};