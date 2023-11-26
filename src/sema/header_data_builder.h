#pragma once

#include "../ast/ast.h"
#include "archive_data.h"
#include "parse_error.h"

#include <string>
#include <string_view>

class lexed_buffer;

class header_data_builder
{
    std::string m_filename;
    lexed_buffer const& m_file;
    header_data m_result;
    std::vector<parse_error> m_errors;

public:
    header_data_builder(std::string const& filename, lexed_buffer const& lexed_buffer);

    auto pre(ast_node const& node) -> bool;
    auto post(ast_node const& node) -> bool;

    auto release_result() -> header_data;
    auto release_errors() -> std::vector<parse_error>;
    auto has_errors() const -> bool;

private:
    // pre
    auto pre_root(u32 idx) -> bool;
    auto pre_statement(u32 idx) -> bool;
    auto pre_decl_var(u32 idx) -> bool;
    auto pre_decl_attr(u32 idx) -> bool;
    auto pre_identifier(u32 idx) -> bool;
    auto pre_regex(u32 idx) -> bool;

    // post
    auto post_root(u32 idx) -> bool;
    auto post_statement(u32 idx) -> bool;
    auto post_decl_var(u32 idx) -> bool;
    auto post_decl_attr(u32 idx) -> bool;
    auto post_identifier(u32 idx) -> bool;
    auto post_regex(u32 idx) -> bool;

    // helpers
    void push_error(std::string const& msg, u32 line_nr);
    void sort_header_by_name();
};