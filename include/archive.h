//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "parse_error.h"

#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <vector>

struct header_data;
struct log_data;
class log_date;

class archive
{
public:
    enum ordering
    {
        UNORDERED,
        BY_DATE,
        BY_FILENAME
    };

    auto has_errors() const -> bool;
    auto get_errors() const -> std::span<parse_error const>;

    void reorder(ordering order);
    auto get_ordering() const -> ordering;

    auto get_header_data() const -> header_data const&;
    auto get_log_data() const -> std::span<std::unique_ptr<log_data> const>;

private:
    void parse_header(std::filesystem::path const& header_path);
    void parse_log_files(std::vector<std::filesystem::path> const& log_paths);

    friend auto parse(std::filesystem::path const& header_path, std::vector<std::filesystem::path> const& log_paths, ordering order)
        -> archive;

    ordering m_ordering = UNORDERED;
    std::vector<parse_error> m_errors;
    std::unique_ptr<header_data> m_header_data;
    std::vector<std::unique_ptr<log_data>> m_log_data;
};