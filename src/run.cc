#include "main.ih"

#include "scanner/scanner.h"

#include "debug.h"

enum
{
    SUCCESS = 0,
    FAIL
};

int run(Options const& opt)
{
    ifstream headerStream(opt.headerFile());

    unique_ptr<HeaderData> headerData = HeaderParser(headerStream).genHeader();

    LogData logData(*headerData);

    for (auto const& logPath : opt.logFiles())
    {
        ifstream logStream(logPath);
        LogParser logParser(logStream, logData);
        logParser.parse();
    }

    logData.sortLinesByDate();

        // set the filter
    std::function<bool(std::string)> filter;

    switch (opt.filterType())
    {
        case FilterType::INCLUSIVE:
            filter = [&opt](std::string str){return regex_match(str, opt.filterRegex());};
        break;
        case FilterType::EXCLUSIVE:
            filter = [&opt](std::string str){return !regex_match(str, opt.filterRegex());};
        break;
        default:
            filter = [](std::string){return true;};
        break;
    }

    Writer writer(opt.outputFile(), filter, ";");
    {
        vector<string> headerLine{"Date", "Var"};
        for (size_t idx = 0; idx < headerData->getAttributes().getCount(); ++idx)
            headerLine.push_back(headerData->getAttributes().getName(idx));
        writer.write(headerLine);
    }

    for (auto const& [date, line] : logData.getLines())
        writer.write(date, line);

    return SUCCESS;
}