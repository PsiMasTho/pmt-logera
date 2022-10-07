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

int main(int argc, char** argv)
{
    Options opts(Args("d:m:E:I:o:", argv));
    opts.debugPrint(cerr);

    unique_ptr<HeaderData> headerData = HeaderParser(opts.headerFile()).gen();

    bool excEncountered = false;
    mutex excMutex;
    mutex pushMutex;
    queue<exception_info> exceptions;

    vector<unique_ptr<LogData>> parsedData;

        // parse logs in paralell
        // avoid par_unseq because we use mutex in LogMerger::makeData()
    for_each(execution::par, begin(opts.logFiles()), end(opts.logFiles()), [&](auto const& pth)
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
            cerr << "Exception encountered in: " << exceptions.front().path << '\n';
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

        // set the filter
    std::function<bool(std::string)> filter;

    switch (opts.filterType())
    {
        case FilterType::INCLUSIVE:
            filter = [&opts](std::string str){return regex_match(str, opts.filterRegex());};
        break;
        case FilterType::EXCLUSIVE:
            filter = [&opts](std::string str){return !regex_match(str, opts.filterRegex());};
        break;
        default:
            filter = [](std::string){return true;};
        break;
    }

        // write header line
    Writer writer(opts.outputFile(), filter, ";");
    {
        vector<string> headerLine{"Date", "Var"};
        for (size_t idx = 0; idx < headerData->getAttributes().getCount(); ++idx)
            headerLine.push_back(headerData->getAttributes().getName(idx));
        writer.write(headerLine);
    }

        // sort the output by date
    stable_sort(begin(parsedData), end(parsedData), [](auto& lhs, auto& rhs){return lhs->getDate() < rhs->getDate();});

    for (auto const& logDataPtr : parsedData)
        for (auto const& logLine  : logDataPtr->getLines())
            writer.write(logDataPtr->getDate(), logLine);

    return SUCCESS;
}