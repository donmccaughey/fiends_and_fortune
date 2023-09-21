#include "str.h"

#include <assert.h>
#include <stdio.h>

#include "fail.h"
#include "xmalloc.h"


char *
str_alloc_centered_and_formatted(int width, char const *format, ...)
{
    assert(width >= 0);
    assert(format);

    va_list arguments;
    va_start(arguments, format);
    char *str = str_alloc_centered_and_formatted_from_va_list(width,
                                                              format,
                                                              arguments);
    va_end(arguments);
    return str;
}


char *
str_alloc_centered_and_formatted_from_va_list(int width,
                                              char const *format,
                                              va_list arguments)
{
    assert(width >= 0);
    assert(format);

    size_t formatted_length = str_formatted_length_from_va_list(format, arguments);
    if (formatted_length >= width) {
        return str_alloc_formatted_from_va_list(format, arguments);
    }
    size_t padding_length = width - formatted_length;
    size_t trailing_length = padding_length / 2;
    size_t leading_length = trailing_length + (padding_length % 2);
    char *str = xmalloc(width + 1);
    memset(str, ' ', leading_length);
    int result = vsprintf(str + leading_length, format, arguments);
    if (result < 0) fail("vsprintf() error");
    memset(str + leading_length + formatted_length, ' ', trailing_length);
    str[leading_length + formatted_length + trailing_length] = '\0';
    return str;
}


extern char *
str_alloc_empty(void);


char *
str_alloc_formatted(char const *format, ...)
{
    assert(format);

    va_list arguments;
    va_start(arguments, format);
    char *str = str_alloc_formatted_from_va_list(format, arguments);
    va_end(arguments);
    return str;
}


char *
str_alloc_formatted_from_va_list(char const *format, va_list arguments)
{
    assert(format);

    char *str = NULL;
    vasprintf_or_die(&str, format, arguments);
    return str;
}


extern bool
str_empty(char const *str);


extern bool
str_eq(char const *str1, char const *str2);


size_t
str_formatted_length(char const *format, ...)
{
    assert(format);

    va_list arguments;
    va_start(arguments, format);
    size_t length = str_formatted_length_from_va_list(format, arguments);
    va_end(arguments);
    return length;
}


size_t
str_formatted_length_from_va_list(char const *format, va_list arguments)
{
    assert(format);

    va_list arguments_copy;
    va_copy(arguments_copy, arguments);
    return vsnprintf(NULL, 0, format, arguments_copy);
}


extern bool
str_not_empty(char const *str);


void
str_realloc_append_formatted(char **str, char const *format, ...)
{
    assert(str);
    assert(format);

    va_list arguments;
    va_start(arguments, format);
    str_realloc_append_formatted_from_va_list(str, format, arguments);
    va_end(arguments);
}


void
str_realloc_append_formatted_from_va_list(char **str,
                                          char const *format,
                                          va_list arguments)
{
    assert(str);
    assert(format);

    size_t append_length = str_formatted_length_from_va_list(format, arguments);
    if ( ! append_length) return;
    
    size_t original_length = *str ? strlen(*str) : 0;
    size_t size = original_length + append_length + 1;
    *str = realloc_or_die(*str, size);
    
    vsprintf(*str + original_length, format, arguments);
}
