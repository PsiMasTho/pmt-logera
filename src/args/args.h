#ifndef INCLUDED_ARGS_ARGS_H
#define INCLUDED_ARGS_ARGS_H

//===---------------------------------------------------------------------===//
///
/// \file
/// This file contains the Args and Opt definitions.
///
//===----------------------------------------------------------------------===//

#include <string>
#include <unordered_map>
#include <utility> // pair
#include <variant> // variant, visit
#include <vector>

// ArgParser interface
template <typename CRTP>
class IArgParser;

class Args
{
public:
    enum ValType
    {
        NONE,
        REQUIRED,
        OPTIONAL
    };

    class Opt
    {
    public:
        using flag_t = std::variant<char, std::string, std::pair<char, std::string>>;

        Opt(ValType type, std::string const& sFlag);
        Opt(ValType type, char cFlag);
        Opt(ValType type, std::string const& sFlag, char cFlag);

        flag_t const& getFlags() const;
        ValType getValType() const;

    private:
        flag_t d_flags;
        ValType d_vType;
    };

    /// \p argv must be null terminated. IArgParser<DefaultArgParser> throws
    // invalid_argument if an unknown option is found or if an option is specified twice
    template <typename ArgParser>
    Args(Opt const* options, size_t optionCount, IArgParser<ArgParser> parser);

    /// \p option short option as char or long option as string
    /// \returns {bool wasSpecified, string const& value}
    /// if the option was not specified, the string reference is to an empty static string.
    /// if the option is invalid, throws invalid_argument
    std::pair<bool, std::string const&> option(std::variant<char, std::string> option) const;

    /// \returns value at argv0
    char const* argv0() const;

private:
    void addOption(std::variant<char, std::string> const& flag, std::string const& val);

    std::vector<std::string> d_vals;
    std::vector<Opt> d_unseenOpts;
    std::unordered_map<char, size_t> d_chFlagToValIdx;
    std::unordered_map<std::string, size_t> d_strFlagToValIdx;
    char const* d_argv0;
};

#include "args.inl"

#endif