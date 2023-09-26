#include "reallocarray.h"

#include <stdint.h>


size_t
array_size(size_t count, size_t element_size, bool *overflow)
{
    static size_t const sqrt_size_max = (size_t)1 << (sizeof(size_t) * 4);
    if (count >= sqrt_size_max || element_size >= sqrt_size_max) {
        if (element_size && count > SIZE_MAX / element_size) {
            if (overflow) *overflow = true;
            return SIZE_MAX;
        }
    }
    if (overflow) *overflow = false;
    return count * element_size;
}


#ifndef HAS_REALLOCARRAY
#include <errno.h>
#include <stdlib.h>

void *
reallocarray(void *memory, size_t count, size_t element_size)
{
    bool too_big;
    size_t size = array_size(count, element_size, &too_big);
    if (too_big) {
        errno = ENOMEM;
        return NULL;
    } else {
        return realloc(memory, size);
    }
}
#endif
