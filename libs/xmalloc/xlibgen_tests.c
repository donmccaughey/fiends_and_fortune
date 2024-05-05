#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "xlibgen.h"


static void
xbasename_tests(void)
{
    char const path[] = "/foo/bar.txt";
    char *base_name = xbasename(path);
    assert(base_name);
    assert(base_name != path);
    assert(0 == strcmp("bar.txt", base_name));
    free(base_name);

    base_name = xbasename(NULL);
    assert(base_name);
    assert(0 == strcmp(".", base_name));
    free(base_name);
}


static void
xdirname_tests(void)
{
    char const path[] = "/foo/bar.txt";
    char *dir_name = xdirname(path);
    assert(dir_name);
    assert(dir_name != path);
    assert(0 == strcmp("/foo", dir_name));
    free(dir_name);

    dir_name = xdirname(NULL);
    assert(dir_name);
    assert(0 == strcmp(".", dir_name));
    free(dir_name);
}


int
main(int argc, char *argv[])
{
    xbasename_tests();
    xdirname_tests();
    return EXIT_SUCCESS;
}
