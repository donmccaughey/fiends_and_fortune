#include "xstring.h"
#include <stdlib.h>
#include <string.h>


void *
memdup(void const *memory, size_t size)
{
    if (!memory) return NULL;

    void *copy = malloc(size);
    if (size && copy) return memcpy(copy, memory, size);

    return copy;
}


char *
xstrdup(const char *string)
{
    if ( ! string) return NULL;
    char *dup = strdup(string);
    if ( ! dup) abort();
    return dup;
}
