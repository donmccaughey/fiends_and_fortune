#ifndef FNF_BASE_STR_H_INCLUDED
#define FNF_BASE_STR_H_INCLUDED


#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


char *
str_alloc_centered_and_formatted(int width, char const *format, ...);

char *
str_alloc_empty(void);

char *
str_alloc_formatted(char const *format, ...);

bool
str_empty(char const *str);

bool
str_eq(char const *str1, char const *str2);

bool
str_not_empty(char const *str);

void
str_realloc_append_formatted(char **str, char const *format, ...);


#endif
