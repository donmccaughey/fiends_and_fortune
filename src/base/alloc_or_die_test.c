#include <assert.h>
#include <base/base.h>


static void
basename_or_die_test(void)
{
    char *name;

    name = basename_or_die(NULL);
    assert(str_eq(".", name));
    free(name);

    name = basename_or_die("");
    assert(str_eq(".", name));
    free(name);

    name = basename_or_die("/");
    assert(str_eq("/", name));
    free(name);

    name = basename_or_die("/foo/bar");
    assert(str_eq("bar", name));
    free(name);

    name = basename_or_die("../foo/bar/baz.txt");
    assert(str_eq("baz.txt", name));
    free(name);
}


void
alloc_or_die_test(void)
{
    basename_or_die_test();
}

