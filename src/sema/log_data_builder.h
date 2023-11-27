#pragma once

#include "../ast/log_nodes.h"
#include "archive_data.h"
#include "parse_error.h"
#include "regex_matcher.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

class lexed_buffer;

class log_data_builder
{
    std::string m_filename;
    lexed_buffer const& m_file;
    log_data m_result;
    std::vector<parse_error> m_errors;
    header_data const& m_header;

    std::vector<regex_matcher> const& m_matchers;

    std::optional<u32> m_active_variable_idx;
    u32 m_entries_created_for_active_variable;

public:
    log_data_builder(std::string const& filename,
                     lexed_buffer const& lexed_buffer,
                     header_data const& header,
                     std::vector<regex_matcher> const& matchers);

    void operator()(log_node const& node);

    void operator()(log_root_node const& node);
    void operator()(log_statement_node const& node);
    void operator()(log_variable_node const& node);
    void operator()(log_ident_value_pair_list_node const& node);
    void operator()(log_date_node const& node);
    void operator()(log_identifier_node const& node);
    void operator()(log_ident_value_pair_node const& node);

    auto release_result() -> log_data;

private:
    void push_error(std::string const& msg, u32 line_nr);

    void set_active_variable_or_err(token_record var);

    auto does_var_exist(std::string const& var_name) -> bool;
    auto does_var_have_attr_idx(u32 var_idx, u32 attr_idx) -> bool;
    auto get_var_idx(std::string const& var_name) -> std::optional<u32>;
    auto get_attr_idx(std::string const& attr_name) -> std::optional<u32>;

    auto validate_attr_val_regex_or_err(u32 attr_idx, std::string const& attr_val) -> bool;

    auto split_ident_value_pair(log_ident_value_pair_node node) -> std::pair<std::string, std::string>;
    auto make_ident_value_pair_list_or_err(std::span<log_ident_value_pair_node const> nodes)
        -> std::optional<std::vector<ident_value_pair>>;
};