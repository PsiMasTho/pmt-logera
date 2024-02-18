#include "logera/flyweight_string.hpp"

#include "logera/overloaded.hpp"

#include <algorithm>

using namespace std;

using compare
    = decltype(overloaded{ [](const unique_ptr<flyweight_string::string_type>& lhs, const string_view& rhs) {
                              return string_view{ lhs->first.get(), static_cast<size_t>(lhs->second) } <=> rhs;
                          },
                           [](const string_view& lhs, const unique_ptr<flyweight_string::string_type>& rhs) {
                               return lhs <=> string_view{ rhs->first.get(), static_cast<size_t>(rhs->second) };
                           } });

flyweight_string::flyweight_string(const string_view str, storage_type& storage)
    : m_str{ nullptr }
{
    const auto it = lower_bound(
        storage.begin(),
        storage.end(),
        str,
        [](const auto& lhs, const auto& rhs) { return compare{}(lhs, rhs) < 0; });

    if (it != end(storage) && compare{}(*it, str) == 0)
        m_str = it->get();
    else
    {
        auto elem = storage.emplace(
            it,
            make_unique<string_type>(make_unique_for_overwrite<char[]>(str.size() + 1), str.size()));
        copy(begin(str), end(str), elem->get()->first.get());
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
