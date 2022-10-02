#ifndef INCLUDED_ATTRIBUTE_H
#define INCLUDED_ATTRIBUTE_H

#include <string>
#include <regex>
#include <functional>

class Attribute
{
    std::string d_name;
    std::function<bool(std::string const&)> d_checkValueRegex;

public:
    explicit Attribute(std::string const& name);
    Attribute(std::string const& name, std::regex expr);
    Attribute(Attribute const&) = default;
    Attribute(Attribute&&) = default;
    ~Attribute() = default;

    std::string const& name() const;
    bool validValue(std::string const& value) const;
};

#endif