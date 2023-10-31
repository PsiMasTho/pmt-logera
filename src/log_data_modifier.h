//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

class header_data;
class log_data;
class date;

class log_data_modifier
{
    log_data* m_target;
    header_data const& m_headerData;

    std::string m_activeVar;

public:
    log_data_modifier(log_data* target, header_data const& hd);

    void set_target(log_data* target);

    void set_date(date const& date);
    void set_filename(std::string const& filename);

    void set_active_var(std::string const& var_name);
    void add_attr_to_new_line(std::string const& attr_name, std::string const& attr_val);
    void add_attr_to_current_line(std::string const& attr_name, std::string const& attr_val);
};