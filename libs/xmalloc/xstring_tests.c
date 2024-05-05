#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xstring.h"


static void
arraydup_tests(void)
{
    int const array[] = { 1, 2, 3, 4, 5 };
    int *copy = arraydup(array, 5, sizeof(int));
    assert(copy);
    assert(1 == copy[0]);
    assert(5 == copy[4]);
    free(copy);

    copy = arraydup(array, 0, sizeof(int));
    assert(copy);
    free(copy);

    errno = 0;
    copy = arraydup(array, SIZE_MAX / 2, sizeof(int));
    assert(!copy);
    assert(ENOMEM == errno);
}


static void
memdup_tests(void)
{
    int const array[] = { 1, 2, 3, 4, 5 };
    int *copy = memdup(array, sizeof(array));
    assert(copy);
    assert(1 == copy[0]);
    assert(5 == copy[4]);
    free(copy);

    copy = memdup(array, 0);
    assert(copy);
    free(copy);
}


static void
xarraydup_tests(void)
{
    int const array[] = { 1, 2, 3, 4, 5 };
    int *copy = xarraydup(array, 5, sizeof(int));
    assert(copy);
    assert(1 == copy[0]);
    assert(5 == copy[4]);
    free(copy);

    copy = xarraydup(array, 0, sizeof(int));
    assert(copy);
    free(copy);

    copy = xarraydup(NULL, 5, sizeof(int));
    assert(copy);
    assert(0 == copy[0]);
    assert(0 == copy[4]);
    free(copy);

    copy = xarraydup(NULL, 0, sizeof(int));
    assert(copy);
    free(copy);
}


static void
xmemdup_tests(void)
{
    int const array[] = { 1, 2, 3, 4, 5 };
    int *copy = xmemdup(array, sizeof(array));
    assert(copy);
    assert(1 == copy[0]);
    assert(5 == copy[4]);
    free(copy);

    copy = xmemdup(array, 0);
    assert(copy);
    free(copy);

    copy = xmemdup(NULL, 5 * sizeof(int));
    assert(copy);
    assert(0 == copy[0]);
    assert(0 == copy[4]);
    free(copy);

    copy = xmemdup(NULL, 0);
    assert(copy);
    free(copy);
}


static void
xstrdup_tests(void)
{
    char *copy = xstrdup("foobar");
    assert(copy);
    assert(0 == strcmp("foobar", copy));
    free(copy);

    copy = xstrdup(NULL);
    assert(copy);
    assert(0 == strcmp("", copy));
    free(copy);
}


static void
xstrndup_tests(void)
{
    char *copy = xstrndup("foobar", 7);
    assert(copy);
    assert(0 == strcmp("foobar", copy));
    free(copy);

    copy = xstrndup("foobar", 6);
    assert(copy);
    assert(0 == strcmp("foobar", copy));
    free(copy);

    copy = xstrndup("foobar", 3);
    assert(copy);
    assert(0 == strcmp("foo", copy));
    free(copy);

    copy = xstrndup("foobar", 0);
    assert(copy);
    assert(0 == strcmp("", copy));
    free(copy);

    copy = xstrndup(NULL, 0);
    assert(copy);
    assert(0 == strcmp("", copy));
    free(copy);

    copy = xstrndup(NULL, 1);
    assert(copy);
    assert(0 == strcmp("", copy));
    free(copy);
}


int
main(int argc, char *argv[])
{
    arraydup_tests();
    memdup_tests();
    xarraydup_tests();
    xmemdup_tests();
    xstrdup_tests();
    xstrndup_tests();
    return EXIT_SUCCESS;
}
