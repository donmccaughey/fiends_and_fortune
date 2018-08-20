#include <stdio.h>
#include <stdlib.h>
#include <base/base.h>


void
ptr_array_test(void);

void
str_test(void);


int
main (int argc, const char *argv[])
{
    ptr_array_test();
    str_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}
