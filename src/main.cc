#include "main.ih"

auto main(int, char** argv) -> int
<%
    unique_ptr<Args> args = make_unique<Args>(argv);

    return run(*args);
%>
