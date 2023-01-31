
template <ArgParser Parser>
Args::Args(Opt const* options, size_t optionCount, Parser parser)
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