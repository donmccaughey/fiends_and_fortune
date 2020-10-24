#include "sort.h"


#include <stdbool.h>
#include <string.h>


static inline void
exchange_items(void *array,
               size_t item_size,
               size_t index1,
               size_t index2)
{
    void *item1 = array + (item_size * index1);
    void *item2 = array + (item_size * index2);
    char temp[item_size];
    memcpy(temp, item1, item_size);
    memcpy(item1, item2, item_size);
    memcpy(item2, temp, item_size);
}


static inline bool
is_less_than(void *array,
             size_t item_size,
             compare_fn compare,
             size_t index1,
             size_t index2)
{
    void *item1 = array + (item_size * index1);
    void *item2 = array + (item_size * index2);
    return compare(item1, item2) < 0;
}


void
insertion_sort(void *array,
               size_t item_count,
               size_t item_size,
               compare_fn compare)
{
    for (size_t i = 1; i < item_count; ++i) {
        for (size_t j = i;
             j > 0 && is_less_than(array, item_size, compare, j, j - 1);
             --j)
        {
            exchange_items(array, item_size, j, j - 1);
        }
    }
}
