#include "flyweight_string.hpp"

#include <algorithm>
#include <cstring>

using namespace std;

flyweight_string::flyweight_string(string_view const str, storage_type& storage)
    : data{ nullptr }
{
    auto const it = lower_bound(
        begin(storage),
        end(storage),
        str,
        [](auto const& a, auto const& b) { return strcmp(a.get(), b.data()) < 0; });

    if (it != end(storage) && it->get() == str)
        data = it->get();
    else
    {
        auto elem = storage.emplace(it, make_unique_for_overwrite<char[]>(str.size() + 1));
        copy(begin(str), end(str), elem->get());
        elem->get()[str.size()] = '\0';
        data                    = elem->get();
    }
}
