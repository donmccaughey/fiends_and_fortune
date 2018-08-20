#ifndef FNF_BASE_STR_H_INCLUDED
#define FNF_BASE_STR_H_INCLUDED


#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>


char *
str_alloc_centered_and_formatted(int width, char const *format, ...);

char *
str_alloc_centered_and_formatted_from_va_list(int width,
                                              char const *format,
                                              va_list arguments);

char *
str_alloc_empty(void);

char *
str_alloc_formatted(char const *format, ...);

char *
str_alloc_formatted_from_va_list(char const *format, va_list arguments);

size_t
str_formatted_length(char const *format, ...);

size_t
str_formatted_length_from_va_list(char const *format, va_list arguments);

bool
str_not_empty(char const *str);

void
str_realloc_append_formatted(char **str, char const *format, ...);

void
str_realloc_append_formatted_from_va_list(char **str,
                                          char const *format,
                                          va_list arguments);


#endif
