//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../external/argparse/argparse.hpp"

#include <fmt/color.h>
#include <fmt/format.h>

#include "archive.h"
#include "archive_data.h"
#include "cmdl_exception.h"
#include "config.h"
#include "csv_generator.h"
#include "log_date.h"

#include <exception>
#include <string>

using namespace std;

namespace
{

argparse::ArgumentParser get_arg_parser()
{
    argparse::ArgumentParser ret("Logera", "1.0", argparse::default_arguments::help);

    ret.set_assign_chars("=");

    ret.add_argument("-d", "--directory").nargs(1).help("directory containing log files");

    ret.add_argument("-m", "--manual").nargs(argparse::nargs_pattern::at_least_one).help("manually select files (e.g. glob)");

    ret.add_argument("-v", "--verbose").help("verbose output").implicit_value(true);

    ret.add_argument("-o", "--output").nargs(1).help("output file. stdout if not specified");

    ret.add_argument("-c", "--color").help("colored errors (on/off)").default_value(string{"on"}).nargs(1);

    return ret;
}

auto parse_args(int argc, char** argv) -> config
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

    return config(cmdl);
}

vector<string> get_header_line(header_data const& header)
{
    vector<string> ret{"date", "var", "filename"};
    for(size_t idx = 0; idx < header.attrs.size(); ++idx)
        ret.push_back(header.attrs[idx].name);

    return ret;
}

void print_error(parse_error const& error, bool color)
{
    if(color)
    {
        fmt::print(stderr, fg(fmt::color::red), "  Error parsing file\n");
        fmt::print(stderr, fg(fmt::color::yellow), "    Filename:      {}\n", error.filename);
        fmt::print(stderr, fg(fmt::color::yellow), "    Error message: {}\n", error.msg);
        fmt::print(stderr, fg(fmt::color::yellow), "    Line:          {}\n\n", error.line_nr);
    }
    else
    {
        fmt::print(stderr, "  Error parsing file\n");
        fmt::print(stderr, "    Filename:      {}\n", error.filename);
        fmt::print(stderr, "    Error message: {}\n", error.msg);
        fmt::print(stderr, "    Line:          {}\n\n", error.line_nr);
    }
}

} // namespace

void print_errors(config const& cfg, span<parse_error const> errors)
{
    bool const color = cfg.color;
    if(color)
        fmt::print(stderr, fg(fmt::color::red), "{} errors encountered:\n", errors.size());
    else
        fmt::print(stderr, "{} errors encountered:\n", errors.size());
    for(auto const& error : errors)
        print_error(error, color);
}

void generate_csv(archive const& ar, config const& cfg)
{
    // write header line
    csv_generator gen(*cfg.output_stream);
    gen.write(get_header_line(ar.get_header_data()));

    // write all other lines
    for(auto const& log_data_ptr : ar.get_log_data())
        for(auto const& entry : log_data_ptr->entries)
            gen.write(log_data_ptr->date, log_data_ptr->filename, entry);
}

int main(int argc, char** argv)
try
{
    config const cfg = parse_args(argc, argv);

    if(cfg.verbose)
        print_config(cfg, cout);

    archive const ar = parse(cfg.header_file, cfg.log_files, archive::ordering::BY_DATE);

    if(ar.has_errors())
    {
        print_errors(cfg, ar.get_errors());
        return EXIT_FAILURE;
    }

    generate_csv(ar, cfg);
}
catch(cmdl_exception const& exc)
{
    fmt::print(stderr, "Command line error encountered:\n  {}\n", exc.what());
    return EXIT_FAILURE;
}
catch(std::exception const& exc)
{
    fmt::print(stderr, "Unhandled std::exception encountered:\n  {}\n", exc.what());
    return EXIT_FAILURE;
}
catch(...)
{
    fmt::print(stderr, "Terminating due to unknown exception.\n");
    return EXIT_FAILURE;
}
