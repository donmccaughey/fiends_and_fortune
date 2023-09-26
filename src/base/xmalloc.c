#include "xmalloc.h"
#include <stdlib.h>


#ifndef HAS_REALLOCARRAY

#include "reallocarray.h"

#endif


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
    void *new_memory;
    if (size) {
        new_memory = realloc(memory, size);
    } else {
        free(memory);
        new_memory = malloc(1);
    }
    if ( ! new_memory) abort();
    return new_memory;
}


void *
xreallocarray(void *memory, size_t count, size_t element_size)
{
    void *new_memory;
    if (count && element_size) {
        new_memory = reallocarray(memory, count, element_size);
    } else {
        free(memory);
        new_memory = malloc(1);
    }
    if ( ! new_memory) abort();
    return new_memory;
}
