#include <assert.h>
#include <base/base.h>


static void
arraydup_test(void)
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
    copy = arraydup(NULL, 5, sizeof(int));
    assert( ! copy);
    assert(0 == errno);

    errno = 0;
    copy = arraydup(array, SIZE_MAX / 2, sizeof(int));
    assert( ! copy);
    assert(ENOMEM == errno);
}


static void
memdup_test(void)
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

    copy = memdup(NULL, sizeof(array));
    assert( ! copy);
}


static void
xarraydup_test(void)
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
    assert( ! copy);
}


static void
xmemdup_test(void)
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

    copy = memdup(NULL, sizeof(array));
    assert( ! copy);
}


static void
xstrdup_test(void)
{
    char *copy = xstrdup("");
    assert(copy);
    assert(0 == strlen(copy));
    free(copy);

    copy = xstrdup("foobar");
    assert(copy);
    assert(6 == strlen(copy));
    free(copy);

    copy = xstrdup(NULL);
    assert(NULL == copy);
}


void
xstring_test(void)
{
    arraydup_test();
    memdup_test();
    xarraydup_test();
    xmemdup_test();
    xstrdup_test();
}
