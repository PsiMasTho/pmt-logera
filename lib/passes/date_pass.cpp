//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/passes/date_pass.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include "logera/errors.hpp"

using namespace std;

/**
 * @brief Checks if the given string is a valid date.
 * @param str The string assumed to be in the format "YYYY-MM-DD".
 * @return True if the given string is a valid date, false otherwise.
 */
static auto is_valid_date(char const* str) -> bool
{
    assert(str != NULL);
    assert(strlen(str) == 10);

    int       y, m, d;
    int const matched = sscanf(str, "%d-%d-%d", &y, &m, &d);

    assert(matched == 3);

    int mon_day[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (y < 1 || m < 1 || m > 12)
        return false;

    bool const leap = (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0));

    mon_day[1] += leap;

    if ((d > mon_day[m - 1]) || (d < 1))
        return false;

    return true;
}

void date_pass::run()
{
    flyweight_string* prev_date = nullptr;

    for (ast::file_node& file : entry_root().children)
    {
        int                                    n_dates_encountered   = 0;
        int                                    n_entries_encountered = 0;
        vector<ast::file_node::children_type*> excess_dates;
        int                                    prev_node                           = -1;
        bool                                   date_not_in_filename_order_reported = false;
        bool                                   no_date_before_entry_reported       = false;
        for (auto& child : file.children)
        {
            int const  cur_node     = child.index();
            auto const cur_location = visit([](auto const& n) { return ast::get_source_location(n); }, child);

            visit(
                overloaded{
                    [&](ast::date_node& n)
                    {
                        ++n_dates_encountered;
                        if (prev_node != -1)
                            errors().emplace_back(error::make_record<error::date_not_first_in_file>(cur_location));
                        if (!is_valid_date(n.record.lexeme.data()))
                            errors().emplace_back(
                                error::make_record<error::invalid_date>(cur_location, to_string_view(n.record.lexeme)));
                        if (prev_date && !date_not_in_filename_order_reported)
                        {
                            if (strcmp(prev_date->data(), n.record.lexeme.data()) > 0)
                            {
                                errors().emplace_back(
                                    error::make_record<error::date_not_in_filename_order>(cur_location));
                                date_not_in_filename_order_reported = true;
                            }
                        }
                        if (n_dates_encountered > 1)
                        {
                            errors().emplace_back(error::make_record<error::multiple_dates>(cur_location));
                            excess_dates.push_back(&child);
                            return;
                        }
                        prev_date = &n.record.lexeme;
                    },
                    [&](ast::entry_node&)
                    {
                        ++n_entries_encountered;
                        if (n_dates_encountered == 0 && !no_date_before_entry_reported)
                        {
                            errors().emplace_back(error::make_record<error::no_date_before_entry>(cur_location));
                            no_date_before_entry_reported = true;
                        }
                    },
                    [&](auto const&) { /*unreachable*/
                                       assert(false);
                    } },
                child);
            prev_node = cur_node;
        }

        if (n_dates_encountered > 0 && n_entries_encountered == 0)
            errors().emplace_back(error::make_record<error::no_entries_in_file>(file.filename));

        // erase excess dates
        sort(begin(excess_dates), end(excess_dates), [](auto const& lhs, auto const& rhs) { return lhs > rhs; });

        for (auto& excess_date : excess_dates)
            file.children.erase(file.children.begin() + distance(&file.children.front(), excess_date));
    }
}