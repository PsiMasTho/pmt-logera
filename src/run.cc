#include "main.ih"

#include "scanner/scanner.h"

#include "debug.h"

enum
{
    SUCCESS = 0,
    FAIL
};

int run()
{
    Options const& opt = Options::instance();
        // set the filter, or nullptr if there is none
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

    ifstream headerStream(opt.headerFile());

    unique_ptr<HeaderData> headerData = HeaderParser(headerStream).genHeader();
    headerData->debugPrint();

    cout << "##############################\n";

    LogData logData(*headerData);

    for (auto const& logPath : opt.logFiles())
    {
        ifstream logStream(logPath);
        LogParser logParser(logStream, logData);
        logParser.parse();
    }

    logData.debugPrint();

    return SUCCESS;
}