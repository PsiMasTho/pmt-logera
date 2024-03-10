#pragma once

#include "logera/errors.hpp"
#include "logera/sema.hpp"

/**
 * @brief Checks that all 'DATE_NODE's are unique, valid, once per file,
 * sorted according to filename and first in the file.
 */
struct date_pass : sema::pass_base<date_pass>
{
    using sema::pass_base<date_pass>::pass_base;
    void run();
};

namespace error
{

struct date_not_in_filename_order : with_column, with_unformatted_msg
{
    date_not_in_filename_order(token::source_location location)
        : with_column(location.filename, location.line, location.column)
        , with_unformatted_msg("date not in filename order")
    {
    }
};

struct date_not_first_in_file : with_column, with_unformatted_msg
{
    date_not_first_in_file(token::source_location location)
        : with_column(location.filename, location.line, location.column)
        , with_unformatted_msg("date not first in file")
    {
    }
};

struct invalid_date : with_column, with_formatted_msg
{
    invalid_date(token::source_location location, std::string_view date)
        : with_column(location.filename, location.line, location.column)
        , with_formatted_msg("invalid date encountered \'", date, "\'")
    {
    }
};

struct multiple_dates : with_column, with_unformatted_msg
{
    multiple_dates(token::source_location location)
        : with_column(location.filename, location.line, location.column)
        , with_unformatted_msg("multiple dates encountered")
    {
    }
};

struct no_date_before_entry : with_column, with_unformatted_msg
{
    no_date_before_entry(token::source_location location)
        : with_column(location.filename, location.line, location.column)
        , with_unformatted_msg("no date encountered before entry")
    {
    }
};

} // namespace error