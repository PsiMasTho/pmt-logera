//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "archive.h"

#include "archive_data.h"
#include "log_date.h"
#include "parser/header_parser.h"
#include "parser/header_parser_context.h"
#include "parser/log_parser.h"
#include "parser/log_parser_context.h"

#include "lexer/lexed_file.h"
#include "lexer/log_lexer.h"
#include "lexer/header_lexer.h"

#include <algorithm>

using namespace std;

auto parse(std::filesystem::path const& header_path, std::vector<std::filesystem::path> const& log_paths, archive::ordering order)
    -> archive
{
    archive ret;
    ret.parse_header(header_path);

    if(ret.has_errors())
        return ret;

    ret.parse_log_files(log_paths);

    if(!ret.has_errors())
        ret.reorder(order);

    return ret;
}

auto archive::has_errors() const -> bool
{
    return !m_errors.empty();
}

auto archive::get_errors() const -> span<parse_error const>
{
    return m_errors;
}

void archive::reorder(ordering order)
{
    if(order == UNORDERED || m_ordering == order)
        return;

    static auto const by_filename = [](auto& lhs, auto& rhs) { return lhs->filename < rhs->filename; };

    static auto const by_date = [](auto& lhs, auto& rhs) { return lhs->date < rhs->date; };

    if(order == BY_DATE)
        sort(begin(m_log_data), end(m_log_data), by_date);
    else if(order == BY_FILENAME)
        sort(begin(m_log_data), end(m_log_data), by_filename);

    m_ordering = order;
}

auto archive::get_ordering() const -> ordering
{
    return m_ordering;
}

auto archive::get_header_data() const -> header_data const&
{
    return *m_header_data;
}

auto archive::get_log_data() const -> std::span<std::unique_ptr<log_data> const>
{
    return m_log_data;
}

void archive::parse_header(std::filesystem::path const& header_path)
{
    header_lexer lexer(header_path.c_str());

    if (lexer.lex() != 0)
    {
        throw std::runtime_error("failed to lex header file"); // todo: get rid of this
        return;
    }

    auto result = lexer.release_result();

    header_parser_context ctx;
    header_parser parser(result, ctx);
    m_header_data = parser.gen();
    move(ctx.get_errors().begin(), ctx.get_errors().end(), back_inserter(m_errors));
}

#include <list>

void archive::parse_log_files(std::vector<std::filesystem::path> const& log_paths)
{
    log_parser_context ctx(m_header_data.get());

    list<lexed_file> files;

    for(auto const& pth : log_paths)
    {
        log_lexer lexer(pth.c_str());

        if (lexer.lex() != 0)
        {
            throw std::runtime_error("failed to lex log file"); // todo: get rid of this
            return;
        }

        files.push_back(lexer.release_result());
        lexed_file_walker walker(files.back());

        log_parser parser(walker, ctx);
        m_log_data.push_back(parser.gen());
    }

    move(ctx.get_errors().begin(), ctx.get_errors().end(), back_inserter(m_errors));
}
