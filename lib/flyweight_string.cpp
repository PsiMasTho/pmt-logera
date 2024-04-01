//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/flyweight_string.hpp"

#include "logera/overloaded.hpp"

#include <algorithm>

using namespace std;

using compare
    = decltype(overloaded{ [](unique_ptr<flyweight_string::string_type> const& lhs, string_view const& rhs) {
                              return string_view{ lhs->first.get(), static_cast<size_t>(lhs->second) } <=> rhs;
                          },
                           [](string_view const& lhs, unique_ptr<flyweight_string::string_type> const& rhs) {
                               return lhs <=> string_view{ rhs->first.get(), static_cast<size_t>(rhs->second) };
                           } });

flyweight_string::flyweight_string(const string_view str, storage_type& storage)
    : m_str{ nullptr }
{
    auto const it = lower_bound(
        storage.begin(),
        storage.end(),
        str,
        [](auto const& lhs, auto const& rhs) { return compare{}(lhs, rhs) < 0; });

    if (it != storage.end() && compare{}(*it, str) == 0)
        m_str = it->get();
    else
    {
        auto elem = storage.emplace(
            it,
            make_unique<string_type>(make_unique_for_overwrite<char[]>(str.size() + 1), str.size()));
        copy(str.begin(), str.end(), elem->get()->first.get());
        elem->get()->first[str.size()] = '\0';
        m_str                          = elem->get();
    }
}

auto flyweight_string::data() const -> char const*
{
    return m_str ? m_str->first.get() : nullptr;
}

auto flyweight_string::size() const -> size_t
{
    return m_str ? m_str->second : 0;
}

auto to_string_view(flyweight_string const& s) -> string_view
{
    return { s.data(), s.size() };
}
