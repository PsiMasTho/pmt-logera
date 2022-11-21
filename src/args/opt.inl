#ifndef INCLUDED_ARGS_OPT_INL
#define INCLUDED_ARGS_OPT_INL

// inline implementations of _Opt functions

constexpr bool _streq(char const* lhs, char const* rhs)
{
    return *lhs == *rhs && (*lhs == '\0' || _streq(lhs + 1, rhs + 1));
}

constexpr _Opt::_Opt(ValType type, char const *name, char ch)
:
	d_name{name},
	d_ch{ch},
	d_type{type}
{};

constexpr _Opt::_Opt(ValType type, char chr)
:
	_Opt(type, nullptr, chr)
{}

constexpr char const *_Opt::getName() const
{
	return d_name;
}

constexpr char _Opt::getCh() const
{
	return d_ch;
}

constexpr _Opt::ValType _Opt::getType()
{
	return d_type;
}

constexpr bool operator==(_Opt const& lhs, _Opt const& rhs)
{
    if (lhs.getCh() == rhs.getCh() && lhs.d_type == rhs.d_type)
        return _streq(lhs.getName(), rhs.getName());
    return false;
}

constexpr bool operator!=(_Opt const& lhs, _Opt const& rhs)
{
    return !(lhs == rhs);
}

#endif