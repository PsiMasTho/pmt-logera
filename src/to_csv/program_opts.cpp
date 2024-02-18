//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "program_opts.hpp"

#include "argparse/argparse.hpp"

#include "cmdl_exception.hpp"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iterator>
#include <ostream>

using namespace std;

namespace
{

vector<filesystem::path> path_vec_from_args(argparse::ArgumentParser const& cmdl)
{
    bool const specified_d = cmdl.present("--directory").has_value();
    bool const specified_m = cmdl.present("--manual").has_value();

    if (specified_m + specified_d != 1)
        throw cmdl_exception("Specify either -m or -d");

    vector<filesystem::path> ret;

    if (specified_m)
    {
        auto paths_as_strings = cmdl.get<vector<string>>("--manual");
        move(begin(paths_as_strings), end(paths_as_strings), back_inserter(ret));
    }
    else
    {
        // check if path is indeed a directory
        if (!filesystem::is_directory(cmdl.get<string>("--directory")))
            throw cmdl_exception("Provided path is not a directory: " + cmdl.get<string>("--directory"));
        // iterate through the directory
        for (auto const& entry : filesystem::directory_iterator(cmdl.get<string>("--directory")))
            ret.push_back(entry.path());
    }

    return ret;
}

bool set_align(argparse::ArgumentParser const& cmdl)
{
    return cmdl.get<bool>("--align");
}

bool set_verbose(argparse::ArgumentParser const& cmdl)
{
    return cmdl.get<bool>("--verbose");
}

bool set_color(argparse::ArgumentParser const& cmdl)
{
    return cmdl.get<bool>("--color");
}

bool set_sort_cols_by_width(argparse::ArgumentParser const& cmdl)
{
    return cmdl.get<bool>("--sort-cols-by-width");
}

unique_ptr<ostream, void (*)(ostream*)> set_output_stream(argparse::ArgumentParser const& cmdl)
{
    if (cmdl.present("--output").has_value())
        return unique_ptr<ostream, void (*)(ostream*)>(
            new ofstream(cmdl.get<string>("--output")),
            [](ostream* ptr) { delete ptr; });
    else
        return unique_ptr<ostream, void (*)(ostream*)>(&cout, [](ostream*) { /* do nothing */ });
}

string set_output_name(argparse::ArgumentParser const& cmdl)
{
    if (cmdl.present("--output").has_value())
        return cmdl.get<string>("--output");
    else
        return "stdout";
}

auto set_input_files(argparse::ArgumentParser const& cmdl) -> vector<filesystem::path>
{
    auto paths = path_vec_from_args(cmdl);

    if (paths.empty())
        throw cmdl_exception("No log files provided");

    return paths;
}

} // namespace

program_opts::program_opts(argparse::ArgumentParser const& cmdl)
    : align{ set_align(cmdl) }
    , verbose{ set_verbose(cmdl) }
    , color{ set_color(cmdl) }
    , sort_cols_by_width{ set_sort_cols_by_width(cmdl) }
    , output_stream{ set_output_stream(cmdl) }
    , output_name{ set_output_name(cmdl) }
    , input_files{ set_input_files(cmdl) }
{
}

program_opts::program_opts()
    : align{ false }
    , verbose{ false }
    , color{ false }
    , sort_cols_by_width{ false }
    , output_stream{ &cout, [](ostream*) { /* do nothing */ } }
    , output_name{ "stdout" }
    , input_files{}
{
}

void print_program_opts(program_opts const& cfg, std::ostream& os)
{
    os << "Verbose output:\n";
    os << "\tAlign: " << boolalpha << cfg.align << '\n';
    os << "\tVerbose: " << boolalpha << cfg.verbose << '\n';
    os << "\tColor: " << boolalpha << cfg.color << '\n';
    os << "\tSort columns by width: " << boolalpha << cfg.sort_cols_by_width << '\n';
    os << "\tOutput: " << cfg.output_name << '\n';
    os << "\tInput file count: " << cfg.input_files.size() << '\n';
    os << "\tInput files:\n";
    for (auto const& log_file : cfg.input_files)
        os << "\t\t" << log_file << '\n';
    os << '\n';
}
