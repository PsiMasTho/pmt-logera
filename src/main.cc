//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "program.h"

#include <cstdlib>
#include <exception>
#include <iostream>


int main(int argc, char** argv)
try
{
    return program(argc, argv).run();
}
catch(std::exception const& exc)
{
    std::cerr << "Unhandled std::exception encountered:\n";
    std::cerr << '\t' << exc.what() << '\n';
    return EXIT_FAILURE;
}
catch(...)
{
    std::cerr << "Terminating due to unknown exception." << '\n';
    return EXIT_FAILURE;
}
