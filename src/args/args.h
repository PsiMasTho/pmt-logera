#ifndef INCLUDED_ARGS_ARGS_H
#define INCLUDED_ARGS_ARGS_H

#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility> // pair
#include <variant> // variant, visit
#include <vector>

class ArgParserCore
{
private:
    char const* const* d_argv0;

protected:
    char const* const* d_argv;

    explicit ArgParserCore(char const* const* argv)
        : d_argv0{argv}
        , d_argv{argv + 1}
    { }

public:
    char const* argv0() const
    {
        return *d_argv0;
    }
};

template <typename T>
concept ArgParser =
    requires(T t) {
        // Returns a flag and its value
        // or an empty string if no value and the flag is a boolean flag.
        {
            t.next()
            } -> std::same_as<std::pair<std::variant<char, std::string>, std::string>>;
        {
            t.done()
            } -> std::same_as<bool>;

        std::is_base_of<ArgParserCore, T>::value;
    };

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

    template <ArgParser Parser>
    Args(Opt const* options, size_t optionCount, Parser parser);

    // option short option as char or long option as string
    // returns {bool wasSpecified, string const& value}
    // if the option was not specified, the string reference is to an empty static string.
    // if the option is invalid, throws invalid_argument
    std::pair<bool, std::string const&> option(std::variant<char, std::string> option) const;

    // returns value at argv0
    char const* argv0() const;

private:
    void addOption(std::variant<char, std::string> const& flag, std::string const& val);

    std::vector<std::string> d_vals;
    std::vector<Opt> d_unseenOpts;
    std::unordered_map<char, size_t> d_chFlagToValIdx;
    std::unordered_map<std::string, size_t> d_strFlagToValIdx;
    char const* d_argv0;
};

#include "args-inl.h"

#endif