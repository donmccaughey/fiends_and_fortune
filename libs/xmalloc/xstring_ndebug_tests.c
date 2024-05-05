#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "xstring.h"


static void
arraydup_tests(void)
{
    errno = 0;
    int *copy = arraydup(NULL, 10, 4);
    assert(!copy);
    assert(EINVAL == errno);
}


static void
memdup_tests(void)
{
    errno = 0;
    int *copy = memdup(NULL, 64);
    assert(!copy);
    assert(EINVAL == errno);
}


int
main(int argc, char *argv[])
{
    arraydup_tests();
    memdup_tests();
    return EXIT_SUCCESS;
}
