//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "config.ih"

namespace
{

vector<filesystem::path> path_vec_from_args(argparse::ArgumentParser const& cmdl)
{
    bool const specified_d = cmdl.present("--directory").has_value();
    bool const specified_m = cmdl.present("--manual").has_value();

    if(specified_m + specified_d != 1)
        throw invalid_argument("Specify either -m or -d");

    vector<filesystem::path> ret;

    if(specified_m)
    {
        auto paths_as_strings = cmdl.get<vector<string>>("--manual");
        move(begin(paths_as_strings), end(paths_as_strings), back_inserter(ret));
    }
    else
    {
        // iterate through the directory
        for(auto const& entry : filesystem::directory_iterator(cmdl.get<string>("--directory")))
            ret.push_back(entry.path());
    }

    return ret;
}

bool setVerbose(argparse::ArgumentParser const& cmdl)
{
    return cmdl.present<bool>("--verbose").has_value();
}

bool setColor(argparse::ArgumentParser const& cmdl)
{
    auto value = cmdl.get<string>("--color");

    transform(value.begin(), value.end(), value.begin(), 
                   [](unsigned char c){return std::tolower(c);});

    if (value == "true" || value == "t" || value == "yes" || value == "y" || value == "1" || value == "on")
        return true;
    if (value == "false" || value == "f" || value == "no" || value == "n" || value == "0" || value == "off")
        return false;
    throw std::runtime_error("invalid value for --color");
}

filesystem::path setHeaderFile(argparse::ArgumentParser const& cmdl)
{
    auto const paths = path_vec_from_args(cmdl);

    // check that there is a single file with a '.lh' extension
    auto const itr = find_if(begin(paths), end(paths), [](auto const& pth) {
        return filesystem::path(pth).extension() == ".lh";
    });

    if(itr == end(paths))
        throw invalid_argument("No header file provided");
    
    size_t const extraHeaders = count_if(next(itr, 1), end(paths), [](auto const& pth) {
        return filesystem::path(pth).extension() == ".lh";
    });

    if(extraHeaders)
        throw invalid_argument("Multiple header files provided");
    
    return *itr;
}

unique_ptr<ostream, void (*)(ostream*)> setOutputStream(argparse::ArgumentParser const& cmdl)
{
    if(cmdl.present("--output").has_value())
        return unique_ptr<ostream, void (*)(ostream*)>(
            new ofstream(cmdl.get<string>("--output")), [](ostream* ptr) { delete ptr; });
    else
        return unique_ptr<ostream, void (*)(ostream*)>(
            &cout, [](ostream*) { /* do nothing */ });
}

string setOutputName(argparse::ArgumentParser const& cmdl)
{
    if(cmdl.present("--output").has_value())
        return cmdl.get<string>("--output");
    else
        return "stdout";
}

vector<filesystem::path> setLogFiles(argparse::ArgumentParser const& cmdl)
{
    auto paths = path_vec_from_args(cmdl);

    paths.erase(
        std::remove_if(begin(paths), end(paths), [](auto const& pth)
        {
            if (pth.extension() == ".lh")
                return true;
            else if (pth.extension() == ".txt")
                return false;
            else
                throw invalid_argument("Invalid file extension");
        }),
        end(paths)
    );

    if(paths.empty())
        throw invalid_argument("No log files provided");

    return paths;
}

}

Config::Config(argparse::ArgumentParser const& cmdl)
    : verbose{setVerbose(cmdl)}
    , color{setColor(cmdl)}
    , headerFile{setHeaderFile(cmdl)}
    , outputStream{setOutputStream(cmdl)}
    , outputName{setOutputName(cmdl)}
    , logFiles{setLogFiles(cmdl)}
{ }

Config::Config()
    : verbose{false}
    , color{false}
    , headerFile{}
    , outputStream{&cout, [](ostream*) { /* do nothing */ }}
    , outputName{"stdout"}
    , logFiles{}
{ }

void print_config(Config const& cfg, std::ostream& os)
{
    os << "Verbose output:\n";
    os << "\tOutput: " << cfg.outputName << '\n';
    os << "\tHeader file: " << cfg.headerFile << '\n';
    os << "\tLog file count: " << cfg.logFiles.size() << '\n';
    os << "\tLog files:\n";
    for(auto const& logFile : cfg.logFiles)
        os << "\t\t" << logFile << '\n';
    os << '\n';
}

