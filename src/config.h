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

struct config
{
    explicit config(argparse::ArgumentParser const& cmdl);
    config();

    bool verbose;
    bool color;
    std::filesystem::path header_file;
    std::unique_ptr<std::ostream, void (*)(std::ostream*)> output_stream; // stdout or file
    std::string output_name; // name of output file or "stdout"
    std::vector<std::filesystem::path> log_files;
};

void print_config(config const& cfg, std::ostream& os);