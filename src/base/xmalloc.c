#include "xmalloc.h"
#include "fail.h"
#include <stdlib.h>


void *
xcalloc(size_t count, size_t element_size)
{
    void *memory = calloc(count, element_size);
    if ( ! memory) fail("calloc(%zu, %zu) failed", count, element_size);
    return memory;
}


void *
xmalloc(size_t size)
{
    void *memory = malloc(size);
    if ( ! memory) fail("malloc(%zu) failed", size);
    return memory;
}


void *
xrealloc(void *memory, size_t size)
{
    memory = realloc(memory, size);
    if ( ! memory) fail("realloc(%p, %zu) failed", memory, size);
    return memory;
}
