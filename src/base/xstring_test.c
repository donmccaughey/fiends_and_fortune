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
xstrdup_test(void)
{
    char *s = xstrdup("");
    assert(s);
    assert(0 == strlen(s));
    free(s);

    s = xstrdup("foobar");
    assert(s);
    assert(6 == strlen(s));
    free(s);

    s = xstrdup(NULL);
    assert(NULL == s);
}


void
xstring_test(void)
{
    memdup_test();
    xstrdup_test();
}
