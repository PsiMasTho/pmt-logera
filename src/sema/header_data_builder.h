#pragma once

#include "../ast/header_nodes.h"
#include "archive_data.h"
#include "parse_error.h"

#include <string>
#include <string_view>
#include <vector>
#include <span>
#include <optional>

class lexed_buffer;

class header_data_builder
{
    std::string m_filename;
    lexed_buffer const& m_file;
    header_data m_result;
    std::vector<parse_error> m_errors;

public:
    header_data_builder(std::string const& filename, lexed_buffer const& lexed_buffer);

    void operator()(header_node const& node);

    void operator()(header_root_node const& node);
    void operator()(header_statement_node const& node);
    void operator()(header_decl_var_node const& node);
    void operator()(header_decl_attr_node const& node);
    void operator()(header_identifier_node const& node){};
    void operator()(header_regex_node const& node){};

    auto release_result() -> header_data;

    auto get_errors() -> std::span<parse_error const>;
    auto has_errors() const -> bool;

private:
    void push_error(std::string const& msg, u32 line_nr);

    auto add_attr_or_err(token_record attr) -> bool;
    auto add_var_or_err(token_record var) -> bool;

    auto add_regex_to_last_attr_or_err(token_record expr) -> bool;
    auto add_attr_to_last_var_or_err(token_record attr) -> bool;

    auto get_attr_idx(std::string const& attr_name) -> std::optional<u32>;

    void sort_header_by_name();
};