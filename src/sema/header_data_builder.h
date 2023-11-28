#pragma once

#include "archive_data.h"
#include "parse_error.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

class lexed_buffer;
class typed_header_ast;

class header_data_builder
{
    std::string m_filename;
    lexed_buffer const& m_lex;
    typed_header_ast const& m_header_ast;
    header_data m_result;
    std::vector<parse_error> m_errors;

public:
    header_data_builder(std::string const& filename, lexed_buffer const& lexed_buffer, typed_header_ast const& header_ast);

    void process();
    auto release_result() -> header_data;

    auto get_errors() -> std::span<parse_error const>;
    auto has_errors() const -> bool;

private:
    void process_decl_attr_statement(decl_attr_statement const& statement);
    void process_decl_var_statement(decl_var_statement const& statement);

    void push_error(std::string const& msg, u32 line_nr);

    auto get_attr_idx(std::string_view attr_name) -> std::optional<u32>;
};