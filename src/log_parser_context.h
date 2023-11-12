//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "archive_data.h"
#include "detail/sparse_array.h"
#include "parser_context_common.h"

#include <optional>
#include <regex>
#include <string>
#include <vector>
#include <memory>

class log_scanner;

class regex_matcher
{
    std::vector<std::regex> m_regexes;

public:
    regex_matcher() = default;
    void add_regex(std::string const& expr);
    bool operator()(std::string const& str) const;
};

class log_parser_context : public error_context
{
    std::unique_ptr<log_data> m_target;
    std::vector<regex_matcher> m_matchers; // access attribute by index

    header_data const* m_header_data;
    log_scanner const* m_scanner; // for getting the current line number

    std::optional<std::size_t> m_active_variable_idx;
    std::size_t m_entries_created_for_active_variable;

public:
    log_parser_context(header_data const* hd);

    void set_date(log_date const& date);
    void set_scanner(log_scanner const& scanner);

    void set_active_variable_or_err(std::string const& var_name);
    void create_entry_or_err(sparse_array<std::string> const& attr_values);

    auto make_attr_value_arr_or_err(std::pair<std::string, std::string> const& attr_value_pair) -> sparse_array<std::string>;
    void update_attr_value_arr_or_err(sparse_array<std::string>& attr_value_arr,
                                        std::pair<std::string, std::string> const& attr_value_pair);

    void at_eof();

    // sets filename, clears state and is ready to parse another log immediately
    std::unique_ptr<log_data> release_log_data();

private:
    void construct_regexes();
    void set_filename_from_scanner();
    void validate_attr_val_regex_or_err(std::size_t attr_idx, std::string const& attr_val);
};
