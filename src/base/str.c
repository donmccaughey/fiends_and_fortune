#include "str.h"

#include <assert.h>
#include <stdio.h>

#include "astr.h"
#include "fail.h"
#include "xmalloc.h"
#include "xstdio.h"


char *
str_alloc_centered_and_formatted(int width, char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    char *str = astr_centered_f_va(width, format, arguments);
    va_end(arguments);
    return str;
}


char *
str_alloc_centered_and_formatted_from_va_list(int width,
                                              char const *format,
                                              va_list arguments)
{
    return astr_centered_f_va(width, format, arguments);
}


char *
str_alloc_empty(void)
{
    return astr_empty();
}


char *
str_alloc_formatted(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    char *str = str_alloc_formatted_from_va_list(format, arguments);
    va_end(arguments);
    return str;
}


char *
str_alloc_formatted_from_va_list(char const *format, va_list arguments)
{
    return astr_f_va(format, arguments);
}


bool
str_empty(char const *str)
{
    return astr_is_empty(str);
}


bool
str_eq(char const *str1, char const *str2)
{
    return astr_eq(str1, str2);
}


size_t
str_formatted_length(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    size_t length = astr_len_f_va(format, arguments);
    va_end(arguments);
    return length;
}


size_t
str_formatted_length_from_va_list(char const *format, va_list arguments)
{
    return astr_len_f_va(format, arguments);
}


bool
str_not_empty(char const *str)
{
    return !str_empty(str);
}


void
str_realloc_append_formatted(char **str, char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    *str = astr_cat_f_va(*str, format, arguments);
    va_end(arguments);
}


void
str_realloc_append_formatted_from_va_list(char **str,
                                          char const *format,
                                          va_list arguments)
{
    *str = astr_cat_f_va(*str, format, arguments);
}
