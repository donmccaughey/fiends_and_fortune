#include "str.h"

#include <stdio.h>

#include "astr.h"


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


void
str_realloc_append_formatted(char **str, char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    *str = astr_cat_f_va(*str, format, arguments);
    va_end(arguments);
}


