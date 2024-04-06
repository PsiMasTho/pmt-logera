//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/passes/date_pass.hpp"

#include "logera/errors.hpp"

#include <array>
#include <cassert>
#include <cctype>
#include <string>
#include <string_view>
#include <tuple>

using namespace std;

static auto date_atoi(char const* str) -> int
{
    int result = 0;
    while (*str != '\0' && *str != '-')
    {
        assert(isdigit(*str));
        result = result * 10 + (*str++ - '0');
    }
    return result;
}

static auto scan_date(string_view const str) -> tuple<int, int, int>
{
    assert(str.size() == 10);
    return { date_atoi(str.data()), date_atoi(str.data() + 5), date_atoi(str.data() + 8) };
}

/**
 * @brief Checks if the given string is a valid date.
 * @param str The string assumed to be in the format "YYYY-MM-DD".
 * @return True if the given string is a valid date, false otherwise.
 */
static auto is_valid_date(string_view const str) -> bool
{
    auto const [y, m, d] = scan_date(str);

    if (y < 1 || m < 1 || m > 12)
        return false;

    array<int, 12> mon_day{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    mon_day[1] += (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)); // leap year

    if ((d > mon_day[m - 1]) || (d < 1))
        return false;

    return true;
}

void date_pass::run()
{
    flyweight_string const* prev_date = nullptr;

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
                    [&](ast::date_node const& n)
                    {
                        ++n_dates_encountered;
                        if (prev_node != -1)
                            errors().emplace_back(error::make_record<error::date_not_first_in_file>(cur_location));
                        if (!is_valid_date(to_string_view(n.record.lexeme)))
                            errors().emplace_back(
                                error::make_record<error::invalid_date>(cur_location, to_string_view(n.record.lexeme)));
                        if (prev_date && !date_not_in_filename_order_reported)
                        {
                            if (to_string_view(*prev_date) > to_string_view(n.record.lexeme))
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
                    [&](ast::entry_node const&)
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