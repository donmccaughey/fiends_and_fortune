#include <stdlib.h>
#include <base/base.h>


void
int_test(void);

void
ptr_array_test(void);

void
result_test(void);

void
rnd_test(void);

void
str_test(void);


int
main(int argc, char *argv[])
{
    int_test();
    ptr_array_test();
    result_test();
    rnd_test();
    str_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
