//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "header_parser.ih"

header_parser::header_parser(std::filesystem::path const& path, header_parser_context& ctx)
    : d_scanner(path)
    , d_matched(d_scanner.matched())
    , m_ctx(ctx)
    , m_error_info {nullopt}
{
}

unique_ptr<header_data> header_parser::gen()
{
    if (parse() == 0) // no error encountered
        return m_ctx.release_header_data();
    else
        return nullptr;
}

parse_error const& header_parser::get_error_info() const
{
    return *m_error_info;
}

void header_parser::error()
{
    string matchTxt = d_matched;
    erase_and_replace(&matchTxt, "\n", "*newline*");

    m_error_info.emplace(parse_error{
        d_scanner.filename(),
        "Unexpected input: (" + matchTxt + ") encountered.",
        d_scanner.lineNr()
    });
}

void header_parser::exceptionHandler(exception const& exc)
{
    m_error_info.emplace(parse_error{
        d_scanner.filename(),
        exc.what(),
        d_scanner.lineNr()
    });
    ABORT();
}