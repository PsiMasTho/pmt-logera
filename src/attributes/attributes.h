#ifndef INCLUDED_ATTRIBUTE_H
#define INCLUDED_ATTRIBUTE_H

#include <functional>
#include <string>
#include <utility>

class Attributes
{
    // function to check a string against a bound regex
    using RegexMatchChecker = std::function<bool(std::string const&)>;
    using Attribute = std::pair<std::string, std::vector<RegexMatchChecker>>;

    std::vector<Attribute> d_attrs;

public:
    Attributes();
    void addAttr(std::string const& name);
    void addRegexToLastAttr(std::string const& expr);

    bool validValue(size_t idx, std::string const& value) const;

    size_t getCount() const;
    size_t getIdx(std::string const& name) const;
    std::string const& getName(size_t idx) const;
};

#endif