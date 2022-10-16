#include "main.ih"

enum
{
    SUCCESS = 0,
    FAIL
};

vector<string> getHeaderLine(HeaderData const& header);

int main(int argc, char** argv)
try
{
    // parse options, may throw
    Options opts(Args("d:m:vo:", argv));

    if(opts.verbose())
        opts.verbosePrint(cerr);

    // parse header
    unique_ptr<HeaderData> headerData = HeaderParser(opts.headerFile()).gen();

    // syntax error encountered
    if(headerData == nullptr)
        return FAIL;

    vector<unique_ptr<LogData>> parsedData;

    // parse log files
    for(auto const& pth : opts.logFiles())
    {
        LogParser logParser(pth, *headerData);
        unique_ptr<LogData> logData = logParser.gen();

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
    Writer writer(opts.outputFile(), ";");
    writer.write(getHeaderLine(*headerData));

    // write all lines
    for(auto const& logDataPtr : parsedData)
        for(auto const& logLine : logDataPtr->getLines())
            writer.write(logDataPtr->getDate(), logLine);

    if(opts.verbose())
        cout << "Done! Output written to: " << opts.outputFile() << '\n';

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