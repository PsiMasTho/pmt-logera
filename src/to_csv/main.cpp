//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "argparse/argparse.hpp"

#include "cmdl_exception.hpp"
#include "csv.hpp"
#include "logera/ast.hpp"
#include "logera/errors.hpp"
#include "logera/io.hpp"
#include "logera/lexer.hpp"
#include "logera/parser.hpp"
#include "logera/sema.hpp"
#include "program_opts.hpp"

#include <cstdio>
#include <exception>
#include <iostream>
#include <span>
#include <string>
#include <vector>

using namespace std;

namespace
{

auto parse_args(int argc, char** argv) -> program_opts
{
    argparse::ArgumentParser cmdl("logera-to-csv", "1.0", argparse::default_arguments::help);

    cmdl.set_assign_chars("=");

    cmdl.add_argument("-a", "--align")
        .help("align generated csv columns")
        .implicit_value(true)
        .default_value(false)
        .nargs(0);

    cmdl.add_argument("-v", "--verbose").help("verbose output").implicit_value(true).default_value(false).nargs(0);

    cmdl.add_argument("--full-paths")
        .help("output full paths in the csv")
        .implicit_value(true)
        .default_value(false)
        .nargs(0);

    cmdl.add_argument("--sort-cols-by-width")
        .help("sort columns by width, widest to the right")
        .implicit_value(true)
        .default_value(false)
        .nargs(0);

    cmdl.add_argument("-d", "--directory").nargs(1).help("directory containing log files");

    cmdl.add_argument("-m", "--manual")
        .nargs(argparse::nargs_pattern::at_least_one)
        .help("manually select files (e.g. glob)");

    cmdl.add_argument("-o", "--output").nargs(1).help("output file. stdout if not specified");

    try
    {
        cmdl.parse_args(argc, argv); // may throw
    }
    catch (std::exception const& e)
    {
        throw cmdl_exception(e.what());
    }

    return program_opts(cmdl);
}

auto get_csv_emitter_flags(program_opts const& opts) -> csv::flags
{
    csv::flags ret = csv::flags::NONE;

    auto const& combine_flags = [&]<csv::flags flag>(csv::flags& ret, bool const b)
    {
        if (b)
            ret = static_cast<csv::flags>(static_cast<int>(ret) | static_cast<int>(flag));
    };

    combine_flags.operator()<csv::flags::ALIGN>(ret, opts.align);
    combine_flags.operator()<csv::flags::SORT_COLS_BY_WIDTH>(ret, opts.sort_cols_by_width);

    return ret;
}

void print_error(error::record_base const& e)
{
    auto const filename_field = [&]() -> string_view
    {
        if (auto const filename = e.filename())
            return io::basename_from_path(*filename);
        return "N/A";
    }();

    auto const position_field = [&]() -> string
    {
        if (auto const line = e.line())
        {
            if (auto const column = e.column())
                return error::concat("l:", *line, " c:", *column);
            else
                return error::concat("c:", *line);
        }
        return "N/A";
    }();

    fprintf(
        stderr,
        "%s",
        error::concat("[", filename_field, "] (", position_field, ") ", e.msg().value_or(""), "\n").c_str());
}

void print_errors(error::container const& errors)
{
    for (auto const& e : errors)
        print_error(*e.get());
}

auto process_file(
    char const*                     filename,
    lexer&                          l,
    string&                         buffer,
    flyweight_string::storage_type& storage,
    error::container&               errors) -> ast::file_node
{
    if (!io::readallf(filename, buffer, errors))
        return {};

    // add an extra newline for the lexer and parser
    buffer.push_back('\n');

    l.set_buffer(filename, buffer.data(), buffer.size());

    parser p(l, storage, errors);
    return p.parse();
}

void emit_csv(
    ast::multifile_node const& multifile,
    ast::multifile_node const& decl_attrs,
    program_opts const&        opts,
    ostream&                   os)
{
    csv::emitter emitter(get_csv_emitter_flags(opts), 3);

    // construct header row
    vector<string_view> header{ "date", "filename", "var" };
    header.reserve(decl_attrs.children.size() + 3);

    auto const flattened_attrs = [&decl_attrs]
    {
        vector<string_view> ret;
        for (auto const& file : decl_attrs.children)
            for (auto const& node : file.children)
                if (auto const* rec = std::get_if<ast::decl_attr_node>(&node))
                    ret.push_back(to_string_view(rec->identifier.record.lexeme));

        sort(ret.begin(), ret.end());
        return ret;
    }();

    header.insert(header.end(), flattened_attrs.begin(), flattened_attrs.end());

    emitter.add_row(header.begin(), header.end());

    // construct entry rows
    auto index_of = [&flattened_attrs](string_view const id)
    {
        auto const it = lower_bound(flattened_attrs.begin(), flattened_attrs.end(), id);
        assert(it != flattened_attrs.end() && *it == id);
        return distance(flattened_attrs.begin(), it) + 3;
    };

    for (auto const& file : multifile.children)
    {
        for (auto const& node : file.children)
        {
            if (auto const* rec = std::get_if<ast::entry_node>(&node))
            {
                ast::date_node const* date = std::get_if<ast::date_node>(&file.children.front());
                assert(date);
                vector<string_view> row;
                row.resize(flattened_attrs.size() + 3);
                row[0] = to_string_view(date->record.lexeme);
                row[1] = opts.full_paths ? file.filename : io::basename_from_path(file.filename);
                row[2] = to_string_view(rec->identifier.record.lexeme);

                for (auto const& ivp : rec->ident_value_pair_list.children)
                    row[index_of(to_string_view(ivp.identifier.record.lexeme))]
                        = to_string_view(ivp.attr_value.record.lexeme);

                emitter.add_row(row.begin(), row.end());
            }
        }
    }

    emitter.emit(os);
}

auto process_files(program_opts const& opts) -> int
{
    flyweight_string::storage_type storage;
    string                         buffer;
    lexer                          l;
    error::container               errors;
    ast::multifile_node            multifile;
    for (auto const& filename : opts.input_files)
    {
        ast::file_node file = process_file(filename.c_str(), l, buffer, storage, errors);
        if (!file.children.empty())
            multifile.children.push_back(std::move(file));
    }

    storage.shrink_to_fit();

    ast::multifile_node decl_attrs;
    ast::multifile_node decl_vars;

    sema::apply_all_passes({ &multifile, &decl_attrs, &decl_vars }, errors);

    if (!errors.empty())
    {
        print_errors(errors);
        return EXIT_FAILURE;
    }

    emit_csv(multifile, decl_attrs, opts, *opts.output_stream);

    return EXIT_SUCCESS;
}

} // namespace

auto main(int argc, char** argv) -> int
try
{
    program_opts const opts = parse_args(argc, argv);

    if (opts.verbose)
        print_program_opts(opts, cout);

    return process_files(opts);
}
catch (cmdl_exception const& exc)
{
    fprintf(stderr, "Error parsing commandline arguments:\n\t%s\n", exc.what());
    fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
    return EXIT_FAILURE;
}
catch (std::exception const& exc)
{
    fprintf(stderr, "Unhandled std::exception encountered:\n  %s\n", exc.what());
    return EXIT_FAILURE;
}
catch (...)
{
    fprintf(stderr, "Terminating due to unknown exception.\n");
    return EXIT_FAILURE;
}
