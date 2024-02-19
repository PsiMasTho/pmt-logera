#include "logera/passes/file_sort_pass.hpp"

#include "logera/algo.hpp"

using namespace std;

void file_sort_pass::run()
{
    sort(
        begin(entry_root().children),
        end(entry_root().children),
        [](auto const& lhs, auto const& rhs) { return lhs.filename < rhs.filename; });

    auto excess_dupes = algo::excess_duplicates(
        begin(entry_root().children),
        end(entry_root().children),
        [](auto const& lhs, auto const& rhs) { return lhs.filename == rhs.filename; });

    for (auto const& excess_dupe : excess_dupes)
        errors().emplace_back(error::code::SEMA_DUPLICATE_FILENAME, excess_dupe->filename.c_str());

    // erase excess dupes from ast
    sort(begin(excess_dupes), end(excess_dupes), [](auto const& lhs, auto const& rhs) { return lhs > rhs; });

    for (auto const& excess_dupe : excess_dupes)
        entry_root().children.erase(excess_dupe);
}