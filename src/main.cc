//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "../external/argparse/argparse.hpp"
#include "../external/termcolor/termcolor.hpp"

#include "config.h"
#include "archive.h"
#include "csv_generator.h"
#include "header_data.h"
#include "date.h"                               

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

namespace
{

argparse::ArgumentParser get_arg_parser()
{
    argparse::ArgumentParser ret("Logera", "1.0", argparse::default_arguments::help);

    ret.set_assign_chars("=");

    ret.add_argument("-d", "--directory")
        .nargs(1)
        .help("directory containing log files");

    ret.add_argument("-m", "--manual")
        .nargs(argparse::nargs_pattern::at_least_one)
        .help("manually select files (e.g. glob)");

    ret.add_argument("-v", "--verbose").help("verbose output").implicit_value(true);

    ret.add_argument("-o", "--output").nargs(1).help("output file. stdout if not specified");

    ret.add_argument("-c", "--color")
        .help("colored errors (on/off)")
        .default_value(string{"on"})
        .nargs(1);
    
    return ret;
}

vector<string> get_header_line(header_data const& header)
{
    vector<string> ret{"date", "var"};
    for(size_t idx = 0; idx < header.get_attributes().get_count(); ++idx)
        ret.push_back(header.get_attributes().get_name(idx));

    return ret;
}

void print_error(parse_error const& error, bool color)
{
    if (color)
        cerr << termcolor::red;
    cerr << "\tError parsing file\n";
    if (color)
        cerr << termcolor::bright_yellow;
    cerr << "\t\tFilename:      " << error.filename << '\n';
    cerr << "\t\tError message: " << error.msg << '\n';
    cerr << "\t\tLine:          " << error.line_nr << "\n\n";
    if (color)
        cerr << termcolor::reset;
}

} // namespace

void print_errors(config const& cfg, span<parse_error const> errors)
{
    bool const color = cfg.color;
    if (color)
        cerr << termcolor::red;
    cerr << errors.size() << " errors encountered:\n";
    if (color)
        cerr << termcolor::reset;
    for (auto const& error : errors)
        print_error(error, color);
}

void generate_csv(archive const& ar, config const& cfg)
{
    // write header line
    csv_generator gen(*cfg.output_stream);
    gen.write(get_header_line(ar.get_header_data()));

    // write all other lines
    for (auto const& log_data_ptr : ar.get_log_data())
        for (auto const& log_line : log_data_ptr->get_lines())
            gen.write(log_data_ptr->get_date(), log_line);
}


int main(int argc, char** argv)
try
{
    argparse::ArgumentParser cmdl = get_arg_parser();
    try
    {
        cmdl.parse_args(argc, argv); // may throw
    }
    catch(std::exception const& e)
    {
        cerr << "\tError parsing command line arguments\n";
        cerr << "\t\tError message: " << e.what() << "\n\n";
        return EXIT_FAILURE;
    }

    config cfg(cmdl);


    if(cfg.verbose)
        print_config(cfg, cout);
    
    auto ar = parse(cfg.header_file, cfg.log_files, archive::ordering::BY_DATE);

    if (ar.has_errors())
    {
        print_errors(cfg, ar.get_errors());
        return EXIT_FAILURE;
    }
   
    generate_csv(ar, cfg);
}
catch(std::exception const& exc)
{
    std::cerr << "Unhandled std::exception encountered:\n";
    std::cerr << '\t' << exc.what() << '\n';
    return EXIT_FAILURE;
}
catch(...)
{
    std::cerr << "Terminating due to unknown exception." << '\n';
    return EXIT_FAILURE;
}
