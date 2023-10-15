//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_ATTRIBUTES_H
#define INCLUDED_ATTRIBUTES_H

#include <functional>
#include <string>
#include <utility>

class attributes
{
    // Function to check a string against a bound regex
    using RegexMatchChecker = std::function<bool(std::string const&)>;
    using Attribute = std::pair<std::string, std::vector<RegexMatchChecker>>;

    std::vector<Attribute> m_attrs;

public:
    attributes();
    void add_attr(std::string const& name);
    void add_regex_to_last_attr(std::string const& expr);

    bool validValue(size_t idx, std::string const& value) const;

    size_t getCount() const;
    size_t getIdx(std::string const& name) const;
    std::string const& getName(size_t idx) const;
};

#endif