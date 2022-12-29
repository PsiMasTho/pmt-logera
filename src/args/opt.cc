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
