#include "main.ih"

auto main(int argc, char** argv) -> int
try
<%
    Options& opts = Options::initialize(Args("d:m:E:I:o:", argv));
    cout << opts.headerFile() << endl;
%>
catch(char const* e)
<%
    cerr << e << '\n';
%>