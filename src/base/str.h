#ifndef FNF_BASE_STR_H_INCLUDED
#define FNF_BASE_STR_H_INCLUDED


#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


char *
str_alloc_formatted(char const *format, ...);

void
str_realloc_append_formatted(char **str, char const *format, ...);


#endif
