#include "options.ih"
 
Options::Options(Args const& args)
:
    d_headerFile{setHeaderFile(args)},
    d_outputFile{setOutputFile(args)},
    d_logFiles{setLogFiles(args)},
    d_filterType{setFilterType(args)},
    d_filterRegex{setFilterRegex(args)}
{}

void Options::debugPrint(std::ostream& out) const
{
    out << "##############################\n";
    out << "HEADER: " << headerFile() << '\n';
    out << "##############################\n";
    out << "OUTPUT: " << outputFile() << '\n';
    out << "##############################\n";
    for (auto& entry : logFiles())
        out << "LOG: " << entry << '\n';
    out << "##############################\n";
}