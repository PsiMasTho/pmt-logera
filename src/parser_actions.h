//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

class header_data;
class log_data;
class date;

class log_parser_actions
{
    log_data* m_target;
    header_data const* m_header_data;

    std::string m_active_var;

    // todo: needs to create regular expressions from the header data

public:
    log_parser_actions(header_data const* hd);
    log_parser_actions(log_parser_actions const&) = delete;

    void set_target(log_data* target);

    void set_date(date const& date);
    void set_filename(std::string const& filename);

    void set_active_var(std::string const& var_name);
    void add_attr_to_new_line(std::string const& attr_name, std::string const& attr_val);
    void add_attr_to_current_line(std::string const& attr_name, std::string const& attr_val);

private:
    void construct_regexes();
    
};

class header_parser_actions
{
    // attributes m_attrs;
    // var_to_attr_idx::iterator m_last_var_itr;

public:

    header_parser_actions(header_data* target);
    // void add_var(std::string const& var_name);
    // void add_attr(std::string const& attr_name);
    // void add_regex_to_last_attr(std::string const& expr);
    // void add_attr_to_last_var(std::string const& attr_name);

    // bool does_var_have_attr(std::string const& var_name, std::string const& attr_name) const;

    // attributes const& get_attributes() const;
};