#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "astr.h"


static void
test_astr_cat_f(void)
{
    char *s = astr_f("foo");

    char *new_s = astr_cat_f(s, "bar %i", 42);

    assert(astr_eq("foobar 42", new_s));
    free(new_s);
}


static void
test_astr_cat_f_for_empty_format(void)
{
    char *s = astr_f("foo");

    char *new_s = astr_cat_f(s, "%s", "");

    assert(s == new_s);
    assert(astr_eq("foo", new_s));
    free(new_s);
}


static void
test_astr_cat_f_for_NULL_string(void)
{
    char *new_s = astr_cat_f(NULL, "bar %i", 42);

    assert(astr_eq("bar 42", new_s));
    free(new_s);
}


static void
test_astr_centered_f(void)
{
    char *s = astr_centered_f(5, "foobar");
    assert(astr_eq("foobar", s));
    free(s);

    s = astr_centered_f(6, "foobar");
    assert(astr_eq("foobar", s));
    free(s);

    s = astr_centered_f(7, "foobar");
    assert(astr_eq(" foobar", s));
    free(s);

    s = astr_centered_f(8, "foobar");
    assert(astr_eq(" foobar ", s));
    free(s);

    s = astr_centered_f(9, "foobar");
    assert(astr_eq("  foobar ", s));
    free(s);

    s = astr_centered_f(10, "foobar");
    assert(astr_eq("  foobar  ", s));
    free(s);

    s = astr_centered_f(4, "%s", "");
    assert(astr_eq("    ", s));
    free(s);

    s = astr_centered_f(6, "%.2f", 3.14159);
    assert(astr_eq(" 3.14 ", s));
    free(s);
}


static void
test_astr_cmp(void)
{
    assert(0 == astr_cmp(NULL, NULL));
    assert(0 == astr_cmp("foo", "foo"));

    assert(astr_cmp(NULL, "") < 0);
    assert(astr_cmp("", NULL) > 0);

    assert(astr_cmp(NULL, "foo") < 0);
    assert(astr_cmp("foo", NULL) > 0);

    assert(astr_cmp("", "foo") < 0);
    assert(astr_cmp("foo", "") > 0);

    assert(astr_cmp("bar", "foo") < 0);
    assert(astr_cmp("foo", "bar") > 0);

    assert(astr_cmp("foo", "foobar") < 0);
    assert(astr_cmp("foobar", "foo") > 0);
}


static void
test_astr_dup(void)
{
    char *s = astr_dup("foo");
    assert(astr_eq("foo", s));
    free(s);
}


static void
test_astr_dup_for_NULL_string(void)
{
    char *s = astr_dup(NULL);
    assert(astr_eq("", s));
    free(s);
}


static void
test_astr_empty(void)
{
    char *s = astr_empty();
    assert(s);
    assert(0 == strlen(s));
    free(s);
}


static void
test_astr_eq(void)
{
    char *s = astr_f("foo");

    assert(astr_eq("foo", s));
    assert( ! astr_eq(s, "bar"));

    free(s);
}


static void
test_astr_eq_for_NULL_string(void)
{
    char *s = astr_f("foo");

    assert( ! astr_eq(s, NULL));
    assert( ! astr_eq(NULL, s));
    assert(astr_eq(NULL, NULL));

    free(s);
}


static void
test_astr_eq_for_empty_strings(void)
{
    char *s1 = astr_empty();
    char *s2 = astr_empty();

    assert(s1 != s2);
    assert(astr_eq(s1, s2));

    free(s2);
    free(s1);
}


static void
test_astr_f(void)
{
    char *s = astr_f("answer: %i", 42);
    assert(s);
    assert(10 == strlen(s));
    assert(astr_eq("answer: 42", s));
    free(s);
}


static void
test_astr_is_empty(void)
{
    assert(astr_is_empty(NULL));
    assert(astr_is_empty(""));
    assert( ! astr_is_empty("foo"));
}


static void
test_astr_len(void)
{
    int length = astr_len("foo");
    assert(3 == length);

    length = astr_len("");
    assert(0 == length);

    length = astr_len(NULL);
    assert(0 == length);
}


static void
test_astr_len_f(void)
{
    errno = 0;
    int length = astr_len_f("answer: %i", 42);
    assert(10 == length);
    assert(0 == errno);
}


int
main(int argc, char *argv[])
{
    test_astr_cat_f();
    test_astr_cat_f_for_empty_format();
    test_astr_cat_f_for_NULL_string();
    test_astr_centered_f();
    test_astr_cmp();
    test_astr_dup();
    test_astr_dup_for_NULL_string();
    test_astr_empty();
    test_astr_eq();
    test_astr_eq_for_NULL_string();
    test_astr_eq_for_empty_strings();
    test_astr_f();
    test_astr_is_empty();
    test_astr_len();
    test_astr_len_f();
    return EXIT_SUCCESS;
}
