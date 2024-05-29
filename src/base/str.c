#include "str.h"

#include <stdio.h>

#include "astr.h"


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
str_alloc_empty(void)
{
    return astr_empty();
}


char *
str_alloc_formatted(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    char *str = astr_f_va(format, arguments);
    va_end(arguments);
    return str;
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


