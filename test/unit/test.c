#include <stdio.h>

#ifdef NDEBUG
#warning "The unit test module must be compiled without NDEBUG defined."
#endif

void test_algo(void);

void test_io(void);

void test_lexer(void);

void test_opaque_vector(void);

int main(void)
{
    test_algo();
    test_opaque_vector();
    test_io();
    test_lexer();
    printf("No assertions triggered. All tests passed.\n");
    return 0;
}
