#include "xmalloc.h"
#include "reallocarray.h"
#include <stdlib.h>


void *
xcalloc(size_t count, size_t element_size)
{
    void *memory = calloc(count, element_size);
    if ( ! memory) abort();
    return memory;
}


void *
xmalloc(size_t size)
{
    void *memory = malloc(size);
    if ( ! memory) abort();
    return memory;
}


void *
xrealloc(void *memory, size_t size)
{
    memory = realloc(memory, size);
    if ( ! memory) abort();
    return memory;
}


void *
xreallocarray(void *memory, size_t count, size_t element_size)
{
    memory = reallocarray(memory, count, element_size);
    if ( ! memory) abort();
    return memory;
}
