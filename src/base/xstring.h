#ifndef FNF_BASE_XSTRING_H_INCLUDED
#define FNF_BASE_XSTRING_H_INCLUDED


#include <stddef.h>


void *
memdup(void const *memory, size_t size);

void *
xmemdup(void const *memory, size_t size);

char *
xstrdup(char const *string);


#endif
