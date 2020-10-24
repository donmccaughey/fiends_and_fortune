#ifndef FNF_BASE_SORT_H_INCLUDED
#define FNF_BASE_SORT_H_INCLUDED


#include <stddef.h>


typedef int
(compare_fn)(void const *, void const *);


void
insertion_sort(void *array,
               size_t item_count,
               size_t item_size,
               compare_fn compare);


#endif
