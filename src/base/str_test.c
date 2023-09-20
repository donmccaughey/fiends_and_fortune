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
    free(str);

    str = str_alloc_centered_and_formatted(6, "foobar");
    assert(0 == strcmp("foobar", str));
    free(str);

    str = str_alloc_centered_and_formatted(7, "foobar");
    assert(0 == strcmp(" foobar", str));
    free(str);

    str = str_alloc_centered_and_formatted(8, "foobar");
    assert(0 == strcmp(" foobar ", str));
    free(str);

    str = str_alloc_centered_and_formatted(9, "foobar");
    assert(0 == strcmp("  foobar ", str));
    free(str);

    str = str_alloc_centered_and_formatted(10, "foobar");
    assert(0 == strcmp("  foobar  ", str));
    free(str);

    str = str_alloc_centered_and_formatted(4, "%s", "");
    assert(0 == strcmp("    ", str));
    free(str);

    str = str_alloc_centered_and_formatted(6, "%.2f", M_PI);
    assert(0 == strcmp(" 3.14 ", str));
    free(str);
}


static void
str_alloc_empty_test(void)
{
    char *str = str_alloc_empty();
    assert(0 == strcmp("", str));
    free(str);
}


static void
str_empty_test(void)
{
    assert(str_empty(NULL));
    assert(str_empty(""));

    assert( ! str_empty("foobar"));
}


static void
str_eq_test(void)
{
    assert(str_eq("", ""));
    assert(str_eq("foo", "foo"));

    assert( ! str_eq("foo", "bar"));
    assert( ! str_eq("foo", "foobar"));
}


static void
str_formatted_length_test(void)
{
    size_t length = str_formatted_length("");
    assert(0 == length);

    length = str_formatted_length("foobar");
    assert(6 == length);

    length = str_formatted_length("%i", 123);
    assert(3 == length);

    length = str_formatted_length("%s = %0.2f", "pi", M_PI);
    assert(9 == length); // "pi = 3.14"
}


static void
str_not_empty_test(void)
{
    assert(str_not_empty("foobar"));

    assert( ! str_not_empty(NULL));
    assert( ! str_not_empty(""));
}

static void
str_realloc_append_formatted_test(void)
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

    free(str);
}


void
str_test(void)
{
    str_alloc_centered_and_formatted_test();
    str_alloc_empty_test();
    str_empty_test();
    str_eq_test();
    str_formatted_length_test();
    str_not_empty_test();
    str_realloc_append_formatted_test();
}
