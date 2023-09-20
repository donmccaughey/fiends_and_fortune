#ifndef FNF_BASE_XMALLOC_H_INCLUDED
#define FNF_BASE_XMALLOC_H_INCLUDED


#include <stddef.h>


void *
xcalloc(size_t count, size_t element_size);

void *
xmalloc(size_t size);

void *
xrealloc(void *memory, size_t size);


#endif
