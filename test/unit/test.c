#include <stdio.h>

#ifdef NDEBUG
    #warning "The unit test module must be compiled without NDEBUG defined."
#endif

void test_algo(void);
void test_io(void);
void test_token_record(void);
void test_opaque_vector(void);

int main(void)
{
    test_algo();
    test_io();
    test_opaque_vector();
    printf("No assertions triggered. All tests passed.\n");
    return 0;
}
