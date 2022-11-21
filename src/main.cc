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
    Args::Opt(Args::Opt::ValType::REQUIRED, "directory", 'd'),
    Args::Opt(Args::Opt::ValType::REQUIRED, "manual", 'm'),
    Args::Opt(Args::Opt::ValType::NONE, "verbose", 'v'),
    Args::Opt(Args::Opt::ValType::NONE, "output", 'o'),
};
size_t const nOptions = size(options);
} // namespace

int main(int, char** argv)
try
{
    // parse options, may throw
    Config cfg(Args(options, nOptions, argv));

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
    Writer writer(cfg.outputFile(), ";");
    writer.write(getHeaderLine(*headerData));

    // write all lines
    for(auto const& logDataPtr : parsedData)
        for(auto const& logLine : logDataPtr->getLines())
            writer.write(logDataPtr->getDate(), logLine);

    if(cfg.verbose())
        cout << "Done! Output written to: " << cfg.outputFile() << '\n';

    return SUCCESS;
}
catch(invalid_argument const& exc)
{
    cerr << "Invalid arguments:\n";
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