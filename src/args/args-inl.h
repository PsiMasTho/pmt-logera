//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

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