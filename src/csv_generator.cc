//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "csv_generator.h"

#include <utility>
#include <ostream>

using namespace std;

namespace
{
// A field in an RFC 4180 CSV needs to
// be enclosed in double quotes under certain
// circumstances, for our format, we only
// need to check for a comma.
bool needs_quotes(string const& str)
{
    return str.find(',') != string::npos;
}
} // namespace

csv_generator::csv_generator(ostream& out)
    : m_out(out)
{ }

void csv_generator::write(log_date const& log_date, log_data::entry const& entry)
{
    m_out << log_date.to_string();
    for(size_t idx = 0; idx < entry.capacity(); ++idx)
    {
        if(entry.exists(idx))
        {
            if(needs_quotes(entry.get(idx)))
                m_out << ",\"" << entry.get(idx) << '"';
            else
                m_out << ',' << entry.get(idx);
        }
        else
            m_out << ',';
    }

    m_out << '\n';
}

void csv_generator::write(std::vector<std::string> const& vec)
{
    string delim;
    for(auto const& str : vec)
        if(needs_quotes(str))
            m_out << exchange(delim, ",") << '"' << str << '"';
        else
            m_out << exchange(delim, ",") << str;

    m_out << '\n';
}
