#pragma once

#include "../ast/ast.h"
#include "archive_data.h"

class lexed_buffer;

class log_data_builder
{
    lexed_buffer const& m_file;
    header_data const& m_header;
    log_data m_result;

public:
    log_data_builder(lexed_buffer const& lexed_buffer, header_data const& header);

    void pre(ast_node const& node);
    void post(ast_node const& node);

    auto release_result() -> log_data;
};