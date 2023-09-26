#include "xstring.h"
#include <stdlib.h>
#include <string.h>


void *
memdup(void const *memory, size_t size)
{
    if ( ! memory) return NULL;
    void *copy = malloc(size);
    if (size && copy) return memcpy(copy, memory, size);
    return copy;
}


void *
xmemdup(void const *memory, size_t size)
{
    if ( ! memory) return NULL;
    void *copy = memdup(memory, size);
    if ( ! copy) abort();
    return copy;
}


char *
xstrdup(const char *string)
{
    if ( ! string) return NULL;
    char *copy = strdup(string);
    if ( ! copy) abort();
    return copy;
}
