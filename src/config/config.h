//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_CONFIG_CONFIG_H
#define INCLUDED_CONFIG_CONFIG_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

// fwd declaration
namespace argparse
{
    class ArgumentParser;
}

struct Config
{
    explicit Config(argparse::ArgumentParser const& cmdl);
    Config();

    bool verbose;
    bool color;
    std::filesystem::path headerFile;
    std::unique_ptr<std::ostream, void (*)(std::ostream*)> outputStream; // stdout or file
    std::string outputName; // name of output file or "stdout"
    std::vector<std::filesystem::path> logFiles;
};

void print_config(Config const& cfg, std::ostream& os);

#endif