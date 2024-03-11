//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// clang-format off
#ifdef __INTELLISENSE__
    #include "errors.hpp"
#endif
// clang-format on

#include "flyweight_string.hpp"

namespace error
{

template <typename... Args>
with_formatted_msg::with_formatted_msg(Args&&... args)
    : m_msg{ concat(std::forward<Args>(args)...) }
{
}

template <record T>
auto record_cast(record_base const* r) -> T const*
{
    return dynamic_cast<T const*>(r);
}

template <record T>
auto record_cast(record_base* r) -> T*
{
    return dynamic_cast<T*>(r);
}

template <record T, typename... Args>
auto make_record(Args&&... args) -> std::unique_ptr<T>
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

auto concat(auto&& first, auto&&... rest) -> std::string
{
    auto y = [&]
    {
        if constexpr (requires { std::string{ first }; })
            return std::string{ first };
        else if constexpr (std::same_as<decltype(first), flyweight_string>)
            return to_string_view(first);
        else
            return std::to_string(first);
    }();
    if constexpr (sizeof...(rest) == 0)
        return y;
    else
        return y + concat(std::forward<decltype(rest)>(rest)...);
}

} // namespace error