#ifndef FNF_BASE_XSTRING_H_INCLUDED
#define FNF_BASE_XSTRING_H_INCLUDED


#include <stddef.h>


void *
arraydup(void const *array, size_t count, size_t element_size);

void *
memdup(void const *memory, size_t size);

void *
xarraydup(void const *array, size_t count, size_t element_size);

void *
xmemdup(void const *memory, size_t size);

char *
xstrdup(char const *string);


#endif
