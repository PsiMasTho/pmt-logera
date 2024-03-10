//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "args.hpp"
#include "argparse/argparse.hpp"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iterator>
#include <ostream>

using namespace std;

namespace args
{
invalid_cmdl::invalid_cmdl(string const& what)
    : invalid_argument(what)
{
}

auto make_program_opts(int argc, char** argv) -> program_opts
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

    cmdl.add_argument("--Wno-unordered-dates")
        .help("suppress warnings about unordered dates")
        .implicit_value(true)
        .default_value(false)
        .nargs(0);

    cmdl.add_argument("--Wno-unordered-decls")
        .help("suppress warnings about unordered declarations")
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
        cmdl.parse_args(argc, argv);
    }
    catch (std::exception const& e)
    {
        throw invalid_cmdl(e.what());
    }

    return program_opts{
        .align               = cmdl.get<bool>("--align"),
        .verbose             = cmdl.get<bool>("--verbose"),
        .full_paths          = cmdl.get<bool>("--full-paths"),
        .wno_unordered_dates = cmdl.get<bool>("--Wno-unordered-dates"),
        .wno_unordered_decls = cmdl.get<bool>("--Wno-unordered-decls"),
        .sort_cols_by_width  = cmdl.get<bool>("--sort-cols-by-width"),
        .output_stream       = [&]() -> decltype(program_opts::output_stream)
        {
            if (cmdl.present("--output").has_value())
                return unique_ptr<FILE, void (*)(FILE*)>(
                    fopen(cmdl.get<string>("--output").c_str(), "w"),
                    [](FILE* ptr) { fclose(ptr); });
            else
                return unique_ptr<FILE, void (*)(FILE*)>(stdout, [](FILE*) { /* do nothing */ });
        }(),
        .output_name = [&]() -> decltype(program_opts::output_name)
        {
            if (cmdl.present("--output").has_value())
                return cmdl.get<string>("--output");
            else
                return "stdout";
        }(),
        .input_files = [&]() -> decltype(program_opts::input_files)
        {
            bool const specified_d = cmdl.present("--directory").has_value();
            bool const specified_m = cmdl.present("--manual").has_value();

            if (specified_m + specified_d != 1)
                throw invalid_cmdl("Specify either -m or -d");

            vector<filesystem::path> paths;

            if (specified_m)
            {
                auto paths_as_strings = cmdl.get<vector<string>>("--manual");
                move(begin(paths_as_strings), end(paths_as_strings), back_inserter(paths));
            }
            else
            {
                // check if path is indeed a directory
                if (!filesystem::is_directory(cmdl.get<string>("--directory")))
                    throw invalid_cmdl("Provided path is not a directory: " + cmdl.get<string>("--directory"));
                // iterate through the directory
                for (auto const& entry : filesystem::directory_iterator(cmdl.get<string>("--directory")))
                    paths.push_back(entry.path());
            }

            if (paths.empty())
                throw invalid_cmdl("No log files provided");

            return paths;
        }()
    };
}

void print_program_opts(program_opts const& cfg, FILE* output)
{
    char const* const boolalpha[2] = { "false", "true" };
    fprintf(output, "Verbose output:\n");
    fprintf(output, "\tAlign: %s\n", boolalpha[cfg.align]);
    fprintf(output, "\tVerbose: %s\n", boolalpha[cfg.verbose]);
    fprintf(output, "\tFull paths: %s\n", boolalpha[cfg.full_paths]);
    fprintf(output, "\tSuppress warnings about unordered dates: %s\n", boolalpha[cfg.wno_unordered_dates]);
    fprintf(output, "\tSuppress warnings about unordered declarations: %s\n", boolalpha[cfg.wno_unordered_decls]);
    fprintf(output, "\tSort columns by width: %s\n", boolalpha[cfg.sort_cols_by_width]);
    fprintf(output, "\tOutput: %s\n", cfg.output_name.c_str());
    fprintf(output, "\tInput file count: %zu\n", cfg.input_files.size());
    fprintf(output, "\tInput files:\n");
    for (auto const& log_file : cfg.input_files)
        fprintf(output, "\t\t%s\n", log_file.c_str());
    fprintf(output, "\n");
}

} // namespace args