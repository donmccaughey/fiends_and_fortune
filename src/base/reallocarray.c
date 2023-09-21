#include "reallocarray.h"

#include <stdint.h>
#include <stdlib.h>


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


void *
reallocarray(void *memory, size_t count, size_t element_size)
{
    bool overflow;
    size_t size = array_size(count, element_size, &overflow);
    if (overflow) return NULL;
    return realloc(memory, size);
}
