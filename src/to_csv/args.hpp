//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace args
{

struct invalid_cmdl : public std::invalid_argument
{
    explicit invalid_cmdl(std::string const& what);
};

struct program_opts
{
    bool                                   align;
    bool                                   verbose;
    bool                                   full_paths;
    bool                                   wno_unordered_dates;
    bool                                   wno_unordered_decls;
    bool                                   wno_file_without_entries;
    bool                                   sort_by_width; // sort CSV columns by width
    bool                                   dry_run;
    std::unique_ptr<FILE, void (*)(FILE*)> output_stream; // stdout or file
    std::string                            output_name;   // name of output file or "stdout"
    std::vector<std::filesystem::path>     input_files;
};

auto make_program_opts(int argc, char** argv) -> program_opts;
void print_program_opts(program_opts const& cfg, FILE* output);

} // namespace args