//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "main.ih"

enum
{
    SUCCESS = 0,
    FAIL
};

namespace
{

vector<string> getHeaderLine(HeaderData const& header)
{
    vector<string> ret{"date", "var"};
    for(size_t idx = 0; idx < header.getAttributes().getCount(); ++idx)
        ret.push_back(header.getAttributes().getName(idx));

    return ret;
}

void verbose_print(Config const& cfg, ostream& os)
{
    os << "Verbose output:\n";
    os << "\tOutput: " << cfg.outputName << '\n';
    os << "\tHeader file: " << cfg.headerFile << '\n';
    os << "\tLog file count: " << cfg.logFiles.size() << '\n';
    os << "\tLog files:\n";
    for (auto const& logFile : cfg.logFiles)
        os << "\t\t" << logFile << '\n';
    os << '\n';
}

} // namespace

int main(int argc, char** argv)
try
{
    argparse::ArgumentParser cmdl("Logera", "1.0", argparse::default_arguments::help);
    cmdl.add_argument("-d", "--directory")
        .nargs(1)
        .help("directory containing log files");
    
    cmdl.add_argument("-m", "--manual")
        .nargs(argparse::nargs_pattern::at_least_one)
        .help("one header file and zero or more log files");
    
    cmdl.add_argument("-v", "--verbose")
        .help("verbose output")
        .implicit_value(true);
    
    cmdl.add_argument("-o", "--output")
        .nargs(1)
        .help("output files. stdout if not specified");
    
    cmdl.parse_args(argc, argv); // may throw

    Config cfg(cmdl); // may throw

    if(cfg.verbose)
        verbose_print(cfg, cerr);

    // parse header
    unique_ptr<HeaderData> headerData = HeaderParser(cfg.headerFile).gen();

    // syntax error encountered
    if(headerData == nullptr)
        return FAIL;

    vector<unique_ptr<LogData>> parsedData;

    bool encounteredError = false;

    // parse log files
    for (auto const& pth : cfg.logFiles)
    {
        LogParser logParser(pth, *headerData);
        unique_ptr<LogData> logData = logParser.gen();

        // error encountered
        if (logData == nullptr)
        {
            encounteredError = true;
            auto const& [msg, lineNr] = logParser.getErrorInfo();
            cerr << "Error encountered in file " << pth << " at line " << lineNr << ":\n";
            cerr << '\t' << msg << '\n';
            cerr << '\n';
        }
        
        if (!encounteredError)
            parsedData.push_back(std::move(logData));
    }

    if (encounteredError)
        return FAIL;

    // sort the output by date
    stable_sort(begin(parsedData), end(parsedData), [](auto& lhs, auto& rhs) {
        return lhs->getDate() < rhs->getDate();
    });

    // write header line
    Writer writer(*cfg.outputStream);
    writer.write(getHeaderLine(*headerData));

    // write all other lines
    for (auto const& logDataPtr : parsedData)
        for(auto const& logLine : logDataPtr->getLines())
            writer.write(logDataPtr->getDate(), logLine);

    return SUCCESS;
}
catch(exception const& exc)
{
    cerr << "Exception encountered:\n";
    cerr << '\t' << exc.what() << '\n';
    return FAIL;
}
catch(...)
{
    cerr << "Terminating due to unknown exception.\n";
}
