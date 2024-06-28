#pragma once

#include "logera/fw_decl/fw_decl.hpp"
#include "logera/multifile/multifile.hpp"
#include "logera/path_view/path_view.hpp"

#include <filesystem>
#include <vector>

NS_FW_DECL_CLASS(pmt, i_warning_collector)
NS_FW_DECL_CLASS(pmt, i_task_executor)

namespace pmt
{

class parser
{
public:
  parser(i_warning_collector& warning_collector_, i_task_executor& task_executor_);

  void add_file(std::filesystem::path file_path_);
  auto parse() -> multifile;

private:
  i_warning_collector&               _warning_collector;
  i_task_executor&                   _task_executor;
  std::vector<std::filesystem::path> _files;
};

} // namespace pmt
