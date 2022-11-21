#ifndef INCLUDED_ARGS_ARGS_H
#define INCLUDED_ARGS_ARGS_H

//===---------------------------------------------------------------------===//
///
/// \file
/// This file contains the Args and Opt definitions.
///
//===----------------------------------------------------------------------===//

#include <unordered_map>
#include <string>
#include <variant> // variant, visit()
#include <memory>  // shared_ptr

class _Opt
{
public:
    enum ValType
    {
        NONE,
        REQUIRED,
        OPTIONAL
    };

    constexpr _Opt(ValType type, char const* name, char ch = '\0');
    constexpr _Opt(ValType type, char chr);

	constexpr char const* getName() const;
	constexpr char getCh() const;

	constexpr ValType getType();

    friend constexpr bool operator==(_Opt const&, _Opt const&);
    friend constexpr bool operator!=(_Opt const&, _Opt const&);
	
private:
    char const* d_name;
    char        d_ch;
    ValType     d_type;
};

#include "opt.inl"

class Args
{
	using key_t = std::variant<std::string, char>;
    using val_t = std::pair<bool, std::shared_ptr<std::string>>;
    std::unordered_map<key_t, val_t> d_optionToValueMap;

public:
    using Opt = _Opt;

        /// \p argv must be null terminated. Throws invalid_argument if an unknown option is found
    Args(_Opt const* options, size_t optionCount, char const* const* argv);

        /// \p option short option as char or long option as string
        /// \returns {bool wasSpecified, string const& value}
        /// if the option was not specified, the string reference is to an empty static string.
        /// if the option is invalid, throws invalid_argument
    std::pair<bool, std::string const&> option(key_t const& option) const;

        /// \returns {bool wasSpecified, string const& value} with the value at argv0
    std::pair<bool, std::string const&> argv0() const;

private:
    bool addOpt(std::string const& optStr);

        // creates either a shared_ptr or a nullptr in the map depending on 'wasSpecified'
        // returns that pointer. Has no effect and returns a nullptr if called
        // with the same Opt more than once
    std::string* createStringForOpt(_Opt const& opt, bool wasSpecified);

        // same as option() except the returned string& is non-const
    std::pair<bool, std::string&> rawOption(key_t const& option) const;
        // both d_chr and d_name cannot be null
    std::pair<bool, std::string&> rawOption(_Opt const& option) const;

};

#endif