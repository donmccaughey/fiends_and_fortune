#include <assert.h>
#include <base/base.h>


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
    xstrdup_test();
}
