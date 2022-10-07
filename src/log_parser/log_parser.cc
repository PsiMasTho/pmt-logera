#include "log_parser.ih"

#include "../log_data/log_data.h"
#include "../header_data/header_data.h"
#include "../date/date.h"

LogParser::LogParser(filesystem::path const& path, HeaderData const& headerData)
:
    d_stream(path),
    d_scanner(d_stream),
    d_logDataModifier(nullptr, headerData),
    d_ret{nullptr}
{}

unique_ptr<LogData> LogParser::gen()
{
    d_ret.reset(new LogData);
    d_logDataModifier.setTarget(d_ret.get());
    parse();
    return move(exchange(d_ret, nullptr));
}