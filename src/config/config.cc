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