#include "main.ih"
    

auto main(int argc, char** argv) -> int
try
<%
    Options& opts = Options::initialize(Args("d:m:E:I:o:", argv));
    
    cout << "HEADER: " << opts.headerFile() << '\n';
    cout << "OUTPUT: " << opts.outputFile() << '\n';
    cout << "##############################\n";

    for (auto& entry : opts.logFiles())
        cout << "LOG: " << entry << '\n';

    cout << "##############################\n";

    run();
%>
catch(char const* e)
<%
    cerr << e << '\n';
%>