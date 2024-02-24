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

    cmdl.add_argument("-c", "--color").help("colored errors").implicit_value(true).default_value(false).nargs(0);

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

void print_error(error::record const& e)
{
    printf("Error: %s\n", e.msg.c_str());
}

void print_errors(std::span<error::record const> errors)
{
    for (auto const& e : errors)
        print_error(e);
}

auto process_file(
    char const*                     filename,
    lexer&                          l,
    string&                         buffer,
    flyweight_string::storage_type& storage,
    vector<error::record>&          errors) -> ast::file_node
{
    if (!io::readallf(filename, buffer, errors))
        return {};

    // add an extra newline for the lexer and parser
    buffer.push_back('\n');

    l.set_buffer(filename, buffer.data(), buffer.size());

    parser p(l, storage, errors);
    return p.parse();
}

void generate_csv(ast::multifile_node const& multifile, csv::flags const flags, ostream& os)
{
    csv::emitter emitter(flags);

    for (auto const& file : multifile.children)
    {
        for (auto const& node : file.children)
        {
            if (auto const* rec = std::get_if<ast::entry_node>(&node))
            {
                csv::row row;
                for (auto const& attr : rec->ident_value_pair_list.children)
                    row.push_back(attr.attr_value.record.lexeme.data());
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
    vector<error::record>          errors;
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

    generate_csv(multifile, get_csv_emitter_flags(opts), *opts.output_stream);

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
