#include "options.ih"

Options::Options(Args const& args)
    : d_verbose { setVerbose(args) }
    , d_headerFile { setHeaderFile(args) }
    , d_outputFile { setOutputFile(args) }
    , d_logFiles { setLogFiles(args) }
{
}

void Options::verbosePrint(std::ostream& out) const
{
    out << "Header: " << '\t' << headerFile() << "\n\n";
    out << "Output: " << '\t' << outputFile() << "\n\n";
    out << "Logs:\n";
    for (auto& entry : logFiles()) out << "\t\t" << entry << '\n';
    out << '\n';
}