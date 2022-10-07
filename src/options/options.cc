#include "options.ih"
 
Options::Options(Args const& args)
:
    d_headerFile{setHeaderFile(args)},
    d_outputFile{setOutputFile(args)},
    d_logFiles{setLogFiles(args)}
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