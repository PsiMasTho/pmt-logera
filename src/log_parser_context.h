//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "detail/sparse_array.h"
#include "archive_data.h"

#include <optional>
#include <string>
#include <regex>
#include <vector>

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

    std::optional<std::size_t> m_active_variable_idx;
    std::size_t m_entries_created_for_active_variable;

public:
    log_parser_context(header_data* hd);

    void set_date(log_date const& date);
    void set_filename(std::string const& filename);
    void set_active_variable_or_throw(std::string const& var_name);
    void create_entry_or_throw(sparse_array<std::string> const& attr_values);

    auto make_attr_value_arr_or_throw(std::pair<std::string, std::string> const& attr_value_pair) -> sparse_array<std::string>;
    void update_attr_value_arr_or_throw(sparse_array<std::string>& attr_value_arr, std::pair<std::string, std::string> const& attr_value_pair);

    void at_eof();

        // clears state and is ready to parse another log immediately
    std::unique_ptr<log_data> release_log_data();

private:
    void construct_regexes();
    auto validate_attr_val_regex(std::size_t attr_idx, std::string const& attr_val) const -> bool;
};

