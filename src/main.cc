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

vector<string> getHeaderLine(HeaderData const& header);

namespace
{
Args::Opt const options[]{
    Args::Opt(Args::ValType::REQUIRED, "directory", 'd'),
    Args::Opt(Args::ValType::REQUIRED, "manual", 'm'),
    Args::Opt(Args::ValType::NONE, "verbose", 'v'),
    Args::Opt(Args::ValType::REQUIRED, "output", 'o'),
};
size_t const nOptions = size(options);
} // namespace

int main(int, char** argv)
try
{
    // parse options, may throw
    Config cfg(Args(options, nOptions, DefaultArgParser(argv)));

    if(cfg.verbose())
        cfg.verbosePrint(cerr);

    // parse header
    unique_ptr<HeaderData> headerData = HeaderParser(cfg.headerFile()).gen();

    // syntax error encountered
    if(headerData == nullptr)
        return FAIL;

    vector<unique_ptr<LogData>> parsedData;

    // parse log files
    for(auto const& pth : cfg.logFiles())
    {
        unique_ptr<LogData> logData = LogParser(pth, *headerData).gen();

        // syntax error encountered
        if(logData == nullptr)
            return FAIL;

        parsedData.push_back(move(logData));
    }

    // sort the output by date
    stable_sort(begin(parsedData), end(parsedData), [](auto& lhs, auto& rhs) {
        return lhs->getDate() < rhs->getDate();
    });

    // write header line
    Writer writer(cfg.outputStream());
    writer.write(getHeaderLine(*headerData));

    // write all other lines
    for(auto const& logDataPtr : parsedData)
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

vector<string> getHeaderLine(HeaderData const& header)
{
    vector<string> ret{"Date", "Var"};
    for(size_t idx = 0; idx < header.getAttributes().getCount(); ++idx)
        ret.push_back(header.getAttributes().getName(idx));

    return ret;
}