//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "config.ih"

Config::Config(Args const& args)
    : d_verbose{setVerbose(args)}
    , d_headerFile{setHeaderFile(args)}
    , d_outputStream{setOutputStream(args)}
    , d_outputName{setOutputName(args)}
    , d_logFiles{setLogFiles(args)}
{ }

void Config::verbosePrint(std::ostream& out) const
{
    out << "Header: " << '\t' << headerFile() << "\n\n";

    out << "Output: " << '\t' << d_outputName << "\n\n";
    out << "Logs:\n";
    for(auto const& entry : logFiles())
        out << "\t\t" << entry << '\n';
    out << '\n';
}