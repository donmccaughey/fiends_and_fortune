#include "ptr_array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"


static int const initial_capacity = 256;


void
ptr_array_add(struct ptr_array *ptr_array, void *element)
{
    assert(ptr_array);
    if (ptr_array->count == ptr_array->capacity) {
        int new_capacity = ptr_array->capacity / 2 * 3;
        void *new_elements = xreallocarray(ptr_array->elements,
                                           new_capacity,
                                           sizeof(void *));
        ptr_array->capacity = new_capacity;
        ptr_array->elements = new_elements;
    }
    int next_index = ptr_array->count;
    ptr_array->elements[next_index] = element;
    ++ptr_array->count;
}


struct ptr_array *
ptr_array_alloc(void)
{
    struct ptr_array *ptr_array = xcalloc(1, sizeof(struct ptr_array));
    ptr_array->elements = xcalloc(initial_capacity, sizeof(void *));
    ptr_array->capacity = initial_capacity;
    return ptr_array;
}


void
ptr_array_clear(struct ptr_array *ptr_array,
                ptr_array_free_element_fn free_element)
{
    assert(ptr_array);
    for (int i = 0; i < ptr_array->count; ++i) {
        free_element(ptr_array->elements[i]);
    }
    ptr_array->count = 0;
}


void
ptr_array_free(struct ptr_array *ptr_array)
{
    if (!ptr_array) return;
    free(ptr_array->elements);
    free(ptr_array);
}


int
ptr_array_index_of(struct ptr_array const *ptr_array, void const *element)
{
    assert(ptr_array);
    int index = 0;
    while (index < ptr_array->count) {
        if (element == ptr_array->elements[index]) return index;
        ++index;
    }
    return -1;
}


void *
ptr_array_last_element(struct ptr_array *ptr_array)
{
    assert(ptr_array);
    return ptr_array->count ? ptr_array->elements[ptr_array->count - 1] : NULL;
}


bool
ptr_array_remove(struct ptr_array *ptr_array, void *element)
{
    assert(ptr_array);
    int index = ptr_array_index_of(ptr_array, element);
    if (-1 == index) return false;
    ptr_array_remove_at(ptr_array, index, NULL);
    return true;
}


void
ptr_array_remove_at(struct ptr_array *ptr_array, int index, void **element_out)
{
    assert(ptr_array);
    assert(index >= 0);
    assert(index < ptr_array->count);
    
    if (element_out) *element_out = ptr_array->elements[index];

    --ptr_array->count;
    int last_index = ptr_array->count;
    if (index == last_index) return;

    int next_index = index + 1;
    void *source = &ptr_array->elements[next_index];
    void *destination = &ptr_array->elements[index];
    size_t size = (last_index - index) * sizeof(void *);
    memmove(destination, source, size);
}
