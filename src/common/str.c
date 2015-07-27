#include "str.h"

#include <stdio.h>

#include "alloc_or_die.h"


char *
str_alloc_empty(void)
{
    return calloc_or_die(1, 1);
}


size_t
str_formatted_length(char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    size_t length = str_formatted_length_from_va_list(format, arguments);
    va_end(arguments);
    return length;
}


size_t
str_formatted_length_from_va_list(char const *format, va_list arguments)
{
    va_list arguments_copy;
    va_copy(arguments_copy, arguments);
    return vsnprintf(NULL, 0, format, arguments_copy);
}


char *
str_realloc_append_formatted(char *str, char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    str = str_realloc_append_formatted_from_va_list(str, format, arguments);
    va_end(arguments);
    return str;
}


char *
str_realloc_append_formatted_from_va_list(char *str,
                                          char const *format,
                                          va_list arguments)
{
    size_t append_length = str_formatted_length_from_va_list(format, arguments);
    if ( ! append_length) return str;
    
    size_t original_length = strlen(str);
    size_t size = original_length + append_length + 1;
    str = realloc_or_die(str, size);
    
    va_list arguments_copy;
    va_copy(arguments_copy, arguments);
    vsprintf(str + original_length, format, arguments);
    return str;
}
