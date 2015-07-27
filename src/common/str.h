#ifndef FNF_COMMON_STR_H_INCLUDED
#define FNF_COMMON_STR_H_INCLUDED


#include <stdarg.h>
#include <stddef.h>


char *
str_alloc_empty(void);

size_t
str_formatted_length(char const *format, ...);

size_t
str_formatted_length_from_va_list(char const *format, va_list arguments);

char *
str_realloc_append_formatted(char *str, char const *format, ...);

char *
str_realloc_append_formatted_from_va_list(char *str,
                                          char const *format,
                                          va_list arguments);


#endif
