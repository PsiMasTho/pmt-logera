#include "main.ih"

enum
{
    SUCCESS = 0,
    FAIL
};

struct exception_info
{
    filesystem::path path;
    exception_ptr    exception;
};

vector<string> getHeaderLine(HeaderData const& header);

int main(int argc, char** argv)
try
{
    Options opts(Args("d:m:o:", argv));

    opts.debugPrint(cerr);

    unique_ptr<HeaderData> headerData = HeaderParser(opts.headerFile()).gen();

    mutex excMutex;
    mutex pushMutex;
    queue<exception_info> exceptions;

    vector<unique_ptr<LogData>> parsedData;

        // parse logs in paralell
        // avoid par_unseq because it ignores mutexes
    for_each(execution::seq, begin(opts.logFiles()), end(opts.logFiles()), [&](auto const& pth)
    {
        LogParser logParser(pth, *headerData);
        try
        {
            unique_ptr<LogData> logData = logParser.gen();
            lock_guard const pushGuard(pushMutex);
            parsedData.push_back(move(logData));
        }
        catch (...)
        {
            lock_guard const excGuard(excMutex);
            exceptions.push({pth, current_exception()});
        }
    });

    if (!exceptions.empty())
    {
        while (!exceptions.empty())
        {
            cerr << "Exception encountered when parsing: " << exceptions.front().path << '\n';
            try
            {
                rethrow_exception(exceptions.front().exception);
            }
            catch(string const& exc)
            {
                cerr << '\t' << exc << '\n';
            }
            catch(...)
            {
                cerr << "Unknown exception.\n";
            }
            
            exceptions.pop();
        }
        cerr << "No output was generated.";
        return FAIL;
    }

        // sort the output by date
    stable_sort(begin(parsedData), end(parsedData), [](auto& lhs, auto& rhs){return lhs->getDate() < rhs->getDate();});

        // write header line
    Writer writer(opts.outputFile(), ";");
    writer.write(getHeaderLine(*headerData));

    for (auto const& logDataPtr : parsedData)
        for (auto const& logLine  : logDataPtr->getLines())
            writer.write(logDataPtr->getDate(), logLine);

    return SUCCESS;
}
catch (string const& exc)
{
    cerr << "Exception encountered:\n";
    cerr << '\t' << exc << '\n'; 
}
catch (...)
{
    cerr << "Unknown exception encountered.\n";
}

vector<string> getHeaderLine(HeaderData const& header)
{
    vector<string> ret{"Date", "Var"};
    for (size_t idx = 0; idx < header.getAttributes().getCount(); ++idx)
        ret.push_back(header.getAttributes().getName(idx));

    return ret;
}