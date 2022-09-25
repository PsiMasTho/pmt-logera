#ifndef INCLUDED_ARGS_BASE_H
#define INCLUDED_ARGS_BASE_H

#include <string>
#include <functional>
#include <unordered_set>
#include <optional>

/*
    To use:
    Inherit from this class and:
        Create member std::optional<T> variables in Derived, register with registerArg(...) which associates them 
        with a string name. This can be make convenient with macros or can be done manually.
        Then assign them values with set(string,string). These can be  parsed from argv.
        define static member conversion functions for each type of data e.g.
        template <>
        static int DERIVED_CLASS::cvtFunc<int>(std::string const &valStr);
*/

template <typename Derived>
class ArgsBase
{
private:
    using BoundCvtFunc_t = std::function<void(std::string const&)>;
    std::unordered_map<std::string, BoundCvtFunc_t>   d_optNameToBoundCvtFunc;

protected:
    template <typename Type>
    void registerArg(std::optional<Type>& optVar, std::string const& optName);

        // assign value inside valStr to optVar using the conversion
        // function provided by Derived
    void set(std::string const& optName, std::string const& valStr);
};

#include "args_base.hi"

#endif