#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED


#include <stdarg.h>
#include <stddef.h>


char *
str_alloc_empty(void);

size_t
str_length_of_format(char const *format, ...);

size_t
str_length_of_format_arguments(char const *format, va_list arguments);

char *
str_realloc_append_format(char *str, char const *format, ...);

char *
str_realloc_append_format_arguments(char *str,
                                    char const *format,
                                    va_list arguments);


#endif
