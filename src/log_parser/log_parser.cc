#include "log_parser.ih"

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