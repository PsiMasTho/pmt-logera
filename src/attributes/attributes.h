#ifndef INCLUDED_ATTRIBUTE_H
#define INCLUDED_ATTRIBUTE_H

#include <string>
#include <functional>
#include <utility>
#include <optional>

class Attributes
{
    using CheckStringFun = std::function<bool(std::string const&)>;
    using Attribute = std::pair<std::string, std::vector<CheckStringFun>>;

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