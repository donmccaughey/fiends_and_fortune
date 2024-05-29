#include "astr.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CHECK_ARG(arg, retval) \
    assert(arg); \
    if ( ! (arg)) { \
        errno = EINVAL; \
        return (retval); \
    }


static size_t const SIZE_OF_NUL = sizeof(char);


char *
astr_cat_f(char *s, char const *format, ...)
{
    va_list args;
    va_start(args, format);
    char *new_s = astr_cat_f_va(s, format, args);
    va_end(args);
    return new_s;
}


char *
astr_cat_f_va(char *s, char const *format, va_list args)
{
    int length_to_append = astr_len_f_va(format, args);
    if ( ! length_to_append) return s ? s : astr_empty();

    size_t original_length = astr_len(s);
    size_t new_size = original_length + length_to_append + SIZE_OF_NUL;
    char *resized_s = realloc(s, new_size);
    if ( ! resized_s) abort();

    char *end_of_s = resized_s + original_length;
    va_list args_copy;
    va_copy(args_copy, args);
    int result = vsnprintf(end_of_s, length_to_append + 1, format, args_copy);
    if (result < 0) {
        free(resized_s);
        return astr_empty();
    }
    return resized_s;
}


char *
astr_centered_f(int width, char const *format, ...)
{
    va_list args;
    va_start(args, format);
    char *s = astr_centered_f_va(width, format, args);
    va_end(args);
    return s;
}


char *
astr_centered_f_va(int width, char const *format, va_list args)
{
    CHECK_ARG(format, astr_empty());

    int formatted_length = astr_len_f_va(format, args);
    if (formatted_length >= width) {
        return astr_f_va(format, args);
    }

    int padding_length = width - formatted_length;
    int trailing_length = padding_length / 2;
    int leading_length = padding_length - trailing_length;

    char *s = malloc(width + 1);
    if ( ! s) abort();

    char *leading_start = s;
    char *formatted_start = leading_start + leading_length;
    char *trailing_start = formatted_start + formatted_length;
    char *end = trailing_start + trailing_length;

    memset(leading_start, ' ', leading_length);

    va_list args_copy;
    va_copy(args_copy, args);
    int result = vsnprintf(formatted_start, formatted_length + 1, format, args_copy);
    if (result < 0) {
        free(s);
        return astr_empty();
    }

    memset(trailing_start, ' ', trailing_length);
    *end = '\0';

    return s;
}


int
astr_cmp(char const *s1, char const *s2)
{
    if (s1 == s2) return 0;
    if ( ! s1) return -1;
    if ( ! s2) return 1;
    return strcmp(s1, s2);
}


char *
astr_dup(const char *s)
{
    return astr_f("%s", s ? s : "");
}


char *
astr_empty(void)
{
    char *s = calloc(1, SIZE_OF_NUL);
    if ( ! s) abort();
    return s;
}


bool
astr_eq(char const *s1, char const *s2)
{
    return 0 == astr_cmp(s1, s2);
}


char *
astr_f(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    char *s = astr_f_va(format, arguments);
    va_end(arguments);
    return s;
}


char *
astr_f_va(char const *format, va_list args)
{
    return astr_cat_f_va(NULL, format, args);
}


bool
astr_is_empty(char const *s)
{
    return ! s || ! s[0];
}


int
astr_len(char const *s)
{
    if (s && s[0]) {
        size_t length = strlen(s);
        assert(length <= INT_MAX);
        return (int)length;
    } else {
        return 0;
    }
}


int
astr_len_f(char const *format, ...)
{
    va_list args;
    va_start(args, format);
    int length = astr_len_f_va(format, args);
    va_end(args);
    return length;
}


int
astr_len_f_va(char const *format, va_list args)
{
    CHECK_ARG(format, 0);

    va_list args_copy;
    va_copy(args_copy, args);
    int length = vsnprintf(NULL, 0, format, args_copy);
    return length > 0 ? length : 0;
}
