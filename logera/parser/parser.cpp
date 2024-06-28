#include "parser.hpp"

namespace pmt
{

parser::parser(i_warning_collector& warning_collector_, i_task_executor& task_executor_)
  : _warning_collector(warning_collector_)
  , _task_executor(task_executor_)
{
}

void parser::add_file(std::filesystem::path file_path_)
{
  _files.push_back(file_path_);
}

auto parser::parse() -> multifile
{
}

} // namespace pmt
