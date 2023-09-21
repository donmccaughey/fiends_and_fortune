#include <assert.h>
#include <base/base.h>


void
xbasename_test(void)
{
    char *name = xbasename(NULL);
    assert(name);
    assert(0 == strcmp(".", name));
    free(name);
    
    name = xbasename("");
    assert(name);
    assert(0 == strcmp(".", name));
    free(name);

    char *path = "/";
    name = xbasename(path);
    assert(name);
    assert(name != path);
    assert(0 == strcmp("/", name));
    free(name);

    path = "foo";
    name = xbasename(path);
    assert(name);
    assert(name != path);
    assert(0 == strcmp("foo", name));
    free(name);

    path = "/foo";
    name = xbasename(path);
    assert(name);
    assert(name != &path[1]);
    assert(0 == strcmp("foo", name));
    free(name);

    path = "/foo/";
    name = xbasename(path);
    assert(name);
    assert(0 == strcmp("foo", name));
    free(name);

    path = "/foo/bar";
    name = xbasename(path);
    assert(name);
    assert(0 == strcmp("bar", name));
    free(name);
}
