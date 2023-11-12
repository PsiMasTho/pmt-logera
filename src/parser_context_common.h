//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "parse_error.h"

#include <span>
#include <vector>
#include <utility>

struct header_data;

class error_context
{
    std::vector<parse_error> m_errors;

public:
    template <typename... Args>
    void push_error(Args&&... args);
    auto get_errors() const -> std::span<parse_error const>;
};

template <typename... Args>
inline void error_context::push_error(Args&&... args)
{
    m_errors.emplace_back(std::forward<Args>(args)...);
}

inline auto error_context::get_errors() const -> std::span<parse_error const>
{
    return m_errors;
}

// assuming unsorted data
auto does_var_exist_lin(std::string const& var_name, header_data const& hd) -> bool;
auto does_var_have_attr_lin(std::string const& var_name, std::string const& attr_name, header_data const& hd) -> bool;
auto get_attr_idx_lin(std::string const& attr_name, header_data const& hd) -> std::size_t;
auto get_var_idx_lin(std::string const& var_name, header_data const& hd) -> std::size_t;

// assuming sorted data
auto does_var_exist_bin(std::string const& var_name, header_data const& hd) -> bool;
auto does_var_have_attr_bin(std::string const& var_name, std::string const& attr_name, header_data const& hd) -> bool;
auto get_attr_idx_bin(std::string const& attr_name, header_data const& hd) -> std::size_t;
auto get_var_idx_bin(std::string const& var_name, header_data const& hd) -> std::size_t;

// indexed
auto does_var_have_attr_idx(std::size_t var_idx, std::size_t attr_idx, header_data const& hd) -> bool;