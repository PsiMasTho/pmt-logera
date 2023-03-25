//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "args.ih"

Args::Opt::Opt(ValType type, std::string const& sFlag)
    : d_flags{sFlag}
    , d_vType{type}
{ }

Args::Opt::Opt(ValType type, char cFlag)
    : d_flags{cFlag}
    , d_vType{type}
{ }

Args::Opt::Opt(ValType type, std::string const& sFlag, char cFlag)
    : d_flags{make_pair(cFlag, sFlag)}
    , d_vType{type}
{ }

Args::Opt::flag_t const& Args::Opt::getFlags() const
{
    return d_flags;
}

Args::ValType Args::Opt::getValType() const
{
    return d_vType;
}
