#include <assert.h>
#include <math.h>
#include <base/base.h>


void
str_test(void);


static void
str_alloc_centered_and_formatted_test(void)
{
    char *str = str_alloc_centered_and_formatted(5, "foobar");
    assert(0 == strcmp("foobar", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(6, "foobar");
    assert(0 == strcmp("foobar", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(7, "foobar");
    assert(0 == strcmp(" foobar", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(8, "foobar");
    assert(0 == strcmp(" foobar ", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(9, "foobar");
    assert(0 == strcmp("  foobar ", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(10, "foobar");
    assert(0 == strcmp("  foobar  ", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(4, "%s", "");
    assert(0 == strcmp("    ", str));
    free_or_die(str);

    str = str_alloc_centered_and_formatted(6, "%.2f", M_PI);
    assert(0 == strcmp(" 3.14 ", str));
    free_or_die(str);
}


static void
str_realloc_format_test(void)
{
    char *str = NULL;
    
    str_realloc_append_formatted(&str, "foo");
    assert(0 == strcmp("foo", str));
    
    str_realloc_append_formatted(&str, "%s", "bar");
    assert(0 == strcmp("foobar", str));
    
    str_realloc_append_formatted(&str, "%c%c%c", 'b', 'a', 'z');
    assert(0 == strcmp("foobarbaz", str));
    
    str_realloc_append_formatted(&str, " %i", 42);
    assert(0 == strcmp("foobarbaz 42", str));
    
    str_realloc_append_formatted(&str, " x %.2f", M_PI);
    assert(0 == strcmp("foobarbaz 42 x 3.14", str));
    
    str_realloc_append_formatted(&str, " x %i%%", 42);
    assert(0 == strcmp("foobarbaz 42 x 3.14 x 42%", str));
    
    free_or_die(str);
}


void
str_test(void)
{
    str_realloc_format_test();
    str_alloc_centered_and_formatted_test();
}
