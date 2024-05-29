#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "astr.h"


static void
test_astr_cat_f_for_invalid_format(void)
{
    char *s = astr_f("foo");

    errno = 0;
    char *new_s = astr_cat_f(s, NULL);

    assert(astr_eq("foo", new_s));
    assert(EINVAL == errno);
    free(new_s);
}


static void
test_astr_centered_f_for_invalid_format(void)
{
    errno = 0;
    char *s = astr_centered_f(5, NULL);
    assert(astr_eq("", s));
    assert(EINVAL == errno);
    free(s);
}


static void
test_astr_f_for_invalid_format(void)
{
    errno = 0;
    char *s = astr_f(NULL);
    assert(astr_eq("", s));
    assert(EINVAL == errno);
    free(s);
}


static void
test_astr_len_f_for_invalid_format(void)
{
    errno = 0;
    int length = astr_len_f(NULL);
    assert(0 == length);
    assert(EINVAL == errno);
}


int
main(int argc, char *argv[])
{
    test_astr_cat_f_for_invalid_format();
    test_astr_centered_f_for_invalid_format();
    test_astr_f_for_invalid_format();
    test_astr_len_f_for_invalid_format();
    return EXIT_SUCCESS;
}
