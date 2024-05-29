#include "str.h"

#include <stdarg.h>

#include "astr.h"


void
str_realloc_append_formatted(char **str, char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    *str = astr_cat_f_va(*str, format, arguments);
    va_end(arguments);
}


