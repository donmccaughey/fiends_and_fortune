#include <assert.h>
#include <base/base.h>


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
    memdup_test();
    xmemdup_test();
    xstrdup_test();
}
