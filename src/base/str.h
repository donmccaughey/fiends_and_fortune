#ifndef FNF_BASE_STR_H_INCLUDED
#define FNF_BASE_STR_H_INCLUDED


#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <base/alloc_or_die.h>


char *
str_alloc_centered_and_formatted(int width, char const *format, ...);

char *
str_alloc_centered_and_formatted_from_va_list(int width,
                                              char const *format,
                                              va_list arguments);

inline char *
str_alloc_empty(void)
{
    return (char *)calloc_or_die(1, 1);
}

char *
str_alloc_formatted(char const *format, ...);

char *
str_alloc_formatted_from_va_list(char const *format, va_list arguments);

inline bool
str_empty(char const *str)
{
    return ! str || ! str[0];
}

inline bool
str_eq(char const *str1, char const *str2)
{
    return 0 == strcmp(str1, str2);
}

size_t
str_formatted_length(char const *format, ...);

size_t
str_formatted_length_from_va_list(char const *format, va_list arguments);

inline bool
str_not_empty(char const *str)
{
    return str && str[0];
}

void
str_realloc_append_formatted(char **str, char const *format, ...);

void
str_realloc_append_formatted_from_va_list(char **str,
                                          char const *format,
                                          va_list arguments);


#endif
