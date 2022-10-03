#ifndef INCLUDED_ATTRIBUTE_H
#define INCLUDED_ATTRIBUTE_H

#include <string>
#include <regex>
#include <functional>
#include <optional>

class Attributes
{
    using CheckStringFun = std::function<bool(std::string const&)>;

    std::vector<std::string>    d_names;
    std::vector<CheckStringFun> d_checkValueRegexFuncs;

public:
    void push_back(std::string const& name);
    void push_back(std::string const& name, std::regex expr);

    bool validValue(size_t idx, std::string const& value) const;

    size_t getCount() const;
    size_t getIdx(std::string const& name) const;
    std::string const& getName(size_t idx) const;

private:
    void insertSorted(std::string const& name, CheckStringFun fun);
};

#endif