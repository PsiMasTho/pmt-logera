//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <regex>
#include <vector>

struct header_data;
struct log_data;
class date;

class regex_matcher
{
    std::vector<std::regex> m_regexes;
    
public:
    regex_matcher() = default;
    void add_regex(std::string const& expr);
    bool operator()(std::string const& str) const;
};

class log_parser_context
{
    std::unique_ptr<log_data> m_target;
    header_data* m_header_data;
    std::vector<regex_matcher> m_matchers; // access attribute by index
    std::string m_active_var;

public:
    log_parser_context(header_data* hd);

    void set_date(date const& date);
    void set_filename(std::string const& filename);

    void set_active_var(std::string const& var_name);
    void add_attr_to_new_line(std::string const& attr_name, std::string const& attr_val);
    void add_attr_to_current_line(std::string const& attr_name, std::string const& attr_val);

        // clears state and is ready to parse another log immediately
    std::unique_ptr<log_data> release_log_data();

private:
    void construct_regexes();
};

