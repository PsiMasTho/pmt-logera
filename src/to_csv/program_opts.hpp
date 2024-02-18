//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <filesystem>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

// fwd declaration
namespace argparse
{
class ArgumentParser;
}

struct program_opts
{
    explicit program_opts(argparse::ArgumentParser const& cmdl);
    program_opts();

    bool                                                   align;
    bool                                                   verbose;
    bool                                                   color;
    bool                                                   sort_cols_by_width;
    std::unique_ptr<std::ostream, void (*)(std::ostream*)> output_stream; // stdout or file
    std::string                                            output_name;   // name of output file or "stdout"
    std::vector<std::filesystem::path>                     input_files;
};

void print_program_opts(program_opts const& cfg, std::ostream& os);
