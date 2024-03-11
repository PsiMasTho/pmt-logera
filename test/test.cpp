//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <cstdio>

#ifdef NDEBUG
#warning "The unit test module should be compiled without NDEBUG defined."
#endif

void test_io();

void test_lexer();

int main()
{
    ;
    test_io();
    test_lexer();
    printf("No assertions triggered. All tests passed.\n");
    return 0;
}
