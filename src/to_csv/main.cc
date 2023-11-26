//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../external/argparse/argparse.hpp"

#include <fmt/color.h>
#include <fmt/format.h>

#include "cmdl_exception.h"
#include "csv_emitter.h"
#include "program_opts.h"

#include <logera/archive.h>
#include <logera/archive_data.h>
#include <logera/log_date.h>

#include <exception>
#include <string>

using namespace std;
using fmt::print;

namespace
{

argparse::ArgumentParser get_arg_parser()
{
    argparse::ArgumentParser ret("logera-to-csv", "1.0", argparse::default_arguments::help);

    ret.set_assign_chars("=");

    ret.add_argument("-a", "--align").help("align generated csv columns").implicit_value(true).default_value(false).nargs(0);

    ret.add_argument("-v", "--verbose").help("verbose output").implicit_value(true).default_value(false).nargs(0);

    ret.add_argument("-c", "--color").help("colored errors").implicit_value(true).default_value(false).nargs(0);

    ret.add_argument("--sort-cols-by-width")
        .help("sort columns by width, widest to the right")
        .implicit_value(true)
        .default_value(false)
        .nargs(0);

    ret.add_argument("-d", "--directory").nargs(1).help("directory containing log files");

    ret.add_argument("-m", "--manual").nargs(argparse::nargs_pattern::at_least_one).help("manually select files (e.g. glob)");

    ret.add_argument("-o", "--output").nargs(1).help("output file. stdout if not specified");

    return ret;
}

auto parse_args(int argc, char** argv) -> program_opts
{
    argparse::ArgumentParser cmdl = get_arg_parser();
    try
    {
        cmdl.parse_args(argc, argv); // may throw
    }
    catch(std::exception const& e)
    {
        throw cmdl_exception(e.what());
    }

    return program_opts(cmdl);
}

vector<string> get_header_line(header_data const& header)
{
    static string const s_date = "date";
    static string const s_filename = "filename";
    static string const s_var = "var";

    vector<string> ret{s_date, s_filename, s_var};
    for(size_t idx = 0; idx < header.attrs.size(); ++idx)
        ret.push_back(header.attrs[idx].name);

    return ret;
}

auto get_log_line(log_date const& date, string const& filename, entry_data const& entry) -> vector<string>
{
    vector<string> ret;
    ret.push_back(date.to_string());
    ret.push_back(filename);
    ret.push_back(entry.var_name);
    for(size_t idx = 0; idx < entry.attr_values.capacity(); ++idx)
    {
        if(entry.attr_values.exists(idx))
            ret.push_back(entry.attr_values.get(idx));
        else
            ret.push_back("");
    }

    return ret;
}

auto get_csv_emitter_flags(program_opts const& opts) -> csv_emitter::flags
{
    csv_emitter::flags ret = csv_emitter::flags::NONE;

    ret |= opts.align ? csv_emitter::flags::ALIGN : csv_emitter::flags::NONE;
    ret |= opts.sort_cols_by_width ? csv_emitter::flags::SORT_COLS_BY_WIDTH : csv_emitter::flags::NONE;

    return ret;
}

void print_error(parse_error const& error, bool color)
{
    char const* const err_types[] = {"Semantic error", "Syntax error", "Exception"};
    if(color)
    {
        print(stderr, fg(fmt::color::red), "  Error parsing file\n");
        print(stderr, fg(fmt::color::yellow), "    Type:          {}\n", err_types[static_cast<int>(error.error_type)]);
        print(stderr, fg(fmt::color::yellow), "    Filename:      {}\n", error.filename);
        print(stderr, fg(fmt::color::yellow), "    Error message: {}\n", error.msg);
        print(stderr, fg(fmt::color::yellow), "    Line:          {}\n\n", error.line_nr);
    }
    else
    {
        print(stderr, "  Error parsing file\n");
        print(stderr, "    Filename:      {}\n", error.filename);
        print(stderr, "    Error message: {}\n", error.msg);
        print(stderr, "    Line:          {}\n\n", error.line_nr);
    }
}

} // namespace

void print_errors(program_opts const& opts, span<parse_error const> errors)
{
    bool const color = opts.color;
    if(color)
        print(stderr, fg(fmt::color::red), "{} errors encountered:\n", errors.size());
    else
        print(stderr, "{} errors encountered:\n", errors.size());
    for(auto const& error : errors)
        print_error(error, color);
}

void generate_csv(archive const& ar, program_opts const& opts)
{
    // add header line
    csv_emitter csv(get_csv_emitter_flags(opts));
    csv.add_row(get_header_line(ar.get_header_data()));

    // write all other lines
    for(auto const& log_data_ptr : ar.get_log_data())
        for(auto const& entry : log_data_ptr->entries)
            csv.add_row(get_log_line(log_data_ptr->date, log_data_ptr->filename, entry));

    // write
    csv.emit(*opts.output_stream);
}

int main(int argc, char** argv)
try
{
    program_opts const opts = parse_args(argc, argv);

    if(opts.verbose)
        print_program_opts(opts, cout);

    archive const ar = parse(opts.header_file, opts.log_files, archive::ordering::BY_DATE);

    if(ar.has_errors())
    {
        print_errors(opts, ar.get_errors());
        return EXIT_FAILURE;
    }

    generate_csv(ar, opts);
}
catch(cmdl_exception const& exc)
{
    print(stderr, "Error parsing commandline arguments:\n\t{}\n", exc.what());
    print(stderr, "Try '{} --help' for more information.\n", argv[0]);
    return EXIT_FAILURE;
}
catch(std::exception const& exc)
{
    print(stderr, "Unhandled std::exception encountered:\n  {}\n", exc.what());
    return EXIT_FAILURE;
}
catch(...)
{
    print(stderr, "Terminating due to unknown exception.\n");
    return EXIT_FAILURE;
}
