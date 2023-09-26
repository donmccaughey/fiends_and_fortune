#include "xstring.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "reallocarray.h"


void *
arraydup(void const *array, size_t count, size_t element_size)
{
    bool too_big;
    size_t size = array_size(count, element_size, &too_big);
    if (too_big) {
        errno = ENOMEM;
        return NULL;
    } else {
        return memdup(array, size);
    }
}


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
