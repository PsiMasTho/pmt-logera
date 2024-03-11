//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

namespace error
{

struct record_base
{
    auto filename() const -> std::optional<std::string_view>;
    auto line() const -> std::optional<int>;
    auto column() const -> std::optional<int>;
    auto msg() const -> std::optional<std::string_view>;

    virtual ~record_base() = default;

private:
    virtual auto v_filename() const -> std::optional<std::string_view>;
    virtual auto v_line() const -> std::optional<int>;
    virtual auto v_column() const -> std::optional<int>;
    virtual auto v_msg() const -> std::optional<std::string_view> = 0;
};

struct with_filename : virtual public record_base
{
    std::string m_filename;

protected:
    with_filename(std::string_view filename);

private:
    auto v_filename() const -> std::optional<std::string_view> final;
};

struct with_line : with_filename
{
    int m_line;

protected:
    with_line(std::string_view filename, int line);

private:
    auto v_line() const -> std::optional<int> final;
};

struct with_column : with_line
{
    int m_column;

protected:
    with_column(std::string_view filename, int line, int column);

private:
    auto v_column() const -> std::optional<int> final;
};

struct with_formatted_msg : virtual public record_base
{
    std::string m_msg;

protected:
    template <typename... Args>
    with_formatted_msg(Args&&... args);

private:
    auto v_msg() const -> std::optional<std::string_view> final;
};

class with_unformatted_msg : virtual public record_base
{
    std::string_view m_msg;

protected:
    with_unformatted_msg(std::string_view msg);

private:
    auto v_msg() const -> std::optional<std::string_view> final;
};

template <typename T>
concept record = std::is_base_of_v<record_base, T>;

template <record T>
auto record_cast(record_base const* r) -> T const*;

template <record T>
auto record_cast(record_base* r) -> T*;

using container = std::vector<std::unique_ptr<record_base>>;

template <record T, typename... Args>
auto make_record(Args&&... args) -> std::unique_ptr<T>;

auto concat(auto&& first, auto&&... rest) -> std::string;

} // namespace error

#include "errors-inl.hpp"
