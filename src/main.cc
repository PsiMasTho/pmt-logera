#include "main.ih"

auto main(int argc, char** argv) -> int
try
<%
    Options& opts = Options::initialize(Args("d:m:E:I:o:", argv));
    cout << "HEADER: " << opts.headerFile() << endl;
    cout << "OUTPUT: " << opts.outputFile() << endl;
    cout << "##############################" << endl;

    for (auto& entry : opts.logFiles())
        cout << "LOG: " << entry << endl;

    run();
%>
catch(char const* e)
<%
    cerr << e << '\n';
%>