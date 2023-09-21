#ifndef FNF_BASE_REALLOCARRAY_H_INCLUDED
#define FNF_BASE_REALLOCARRAY_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>


size_t
array_size(size_t count, size_t element_size, bool *overflow);


void *
reallocarray(void *memory, size_t count, size_t element_size);


#endif
