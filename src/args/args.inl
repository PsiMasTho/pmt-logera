template <typename CRTP>
class IArgParser
{
private:
    char const* const* d_argv0;

protected:
    char const* const* d_argv;

public:
    explicit IArgParser(char const* const* argv)
        : d_argv0{argv}
        , d_argv{argv + 1}
    { }

    // Returns a flag and its value
    // or an empty string if no value,
    // advances d_argv
    std::pair<std::variant<char, std::string>, std::string> next()
    {
        return static_cast<CRTP*>(this)->next();
    }

    char const* argv0() const
    {
        return *d_argv0;
    }

    bool done() const
    {
        return !*d_argv;
    }
};

template <typename ArgParser>
Args::Args(Opt const* options, size_t optionCount, IArgParser<ArgParser> parser)
    : d_vals{}
    , d_unseenOpts(options, options + optionCount)
    , d_chFlagToValIdx{}
    , d_strFlagToValIdx{}
    , d_argv0{parser.argv0()}
{
    while(!parser.done())
    {
        auto [flag, val] = parser.next();
        addOption(flag, val);
    }
}