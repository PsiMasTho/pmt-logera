//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/errors.hpp"

namespace error
{

auto record_base::filename() const -> std::optional<std::string_view>
{
    return v_filename();
}

auto record_base::line() const -> std::optional<int>
{
    return v_line();
}

auto record_base::column() const -> std::optional<int>
{
    return v_column();
}

auto record_base::msg() const -> std::optional<std::string_view>
{
    return v_msg();
}

auto record_base::v_filename() const -> std::optional<std::string_view>
{
    return std::nullopt;
}

auto record_base::v_line() const -> std::optional<int>
{
    return std::nullopt;
}

auto record_base::v_column() const -> std::optional<int>
{
    return std::nullopt;
}

with_filename::with_filename(std::string_view filename)
    : m_filename{ filename }
{
}

auto with_filename::v_filename() const -> std::optional<std::string_view>
{
    return m_filename;
}

with_line::with_line(std::string_view filename, int line)
    : with_filename{ filename }
    , m_line{ line }
{
}

auto with_line::v_line() const -> std::optional<int>
{
    return m_line;
}

with_column::with_column(std::string_view filename, int line, int column)
    : with_line{ filename, line }
    , m_column{ column }
{
}

auto with_column::v_column() const -> std::optional<int>
{
    return m_column;
}

auto with_formatted_msg::v_msg() const -> std::optional<std::string_view>
{
    return m_msg;
}

with_unformatted_msg::with_unformatted_msg(std::string_view msg)
    : m_msg{ msg }
{
}

auto with_unformatted_msg::v_msg() const -> std::optional<std::string_view>
{
    return m_msg;
}

} // namespace error