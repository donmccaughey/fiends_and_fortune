#include <stdlib.h>


void
int_test(void);

void
ptr_array_test(void);

void
reallocarray_test(void);

void
result_test(void);

void
rnd_test(void);

void
sort_test(void);

void
str_test(void);

void
xlibgen_test(void);

void
xmalloc_test(void);

void
xstdio_test(void);

void
xstring_test(void);


int
main(int argc, char *argv[])
{
    int_test();
    ptr_array_test();
    reallocarray_test();
    result_test();
    rnd_test();
    sort_test();
    str_test();
    xlibgen_test();
    xmalloc_test();
    xstdio_test();
    xstring_test();
    return EXIT_SUCCESS;
}
