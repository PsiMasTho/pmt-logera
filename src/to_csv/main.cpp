//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "argparse/argparse.hpp"

#include "cmdl_exception.hpp"
#include "csv_emitter.hpp"
#include "program_opts.hpp"

#include <exception>
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

auto get_csv_emitter_flags(program_opts const& opts) -> csv_emitter::flags
{
    csv_emitter::flags ret = csv_emitter::flags::NONE;

    ret |= opts.align ? csv_emitter::flags::ALIGN : csv_emitter::flags::NONE;
    ret |= opts.sort_cols_by_width ? csv_emitter::flags::SORT_COLS_BY_WIDTH : csv_emitter::flags::NONE;

    return ret;
}

} // namespace

auto main(int argc, char** argv) -> int
try
{
    program_opts const opts = parse_args(argc, argv);

    if (opts.verbose)
        print_program_opts(opts, cout);

    // archive const ar = parse(opts.header_file, opts.log_files, archive::ordering::BY_DATE);

    /*
        if (ar.has_errors())
        {
            print_errors(opts, ar.get_errors());
            return EXIT_FAILURE;
        }

        generate_csv(ar, opts);

    */
    return EXIT_SUCCESS;
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
