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
