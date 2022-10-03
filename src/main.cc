#include "main.ih"
    

auto main(int argc, char** argv) -> int
try
<%
    Options& opts = Options::initialize(Args("d:m:E:I:o:", argv));
    opts.debugPrint(cerr);

    run();
%>
catch (char const* e)
<%
    cerr << e << '\n';
%>
catch (string const&e)
<%
    cerr << e << '\n';
%>