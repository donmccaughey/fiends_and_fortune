#ifndef PTR_ARRAY_H_INCLUDED
#define PTR_ARRAY_H_INCLUDED


#include <stdbool.h>


typedef void (*ptr_array_free_element_fn)(void *element);


struct ptr_array {
    void **elements;
    int count;
    int capacity;
};


struct ptr_array *
ptr_array_alloc(void);

void
ptr_array_free(struct ptr_array *ptr_array);

void
ptr_array_add(struct ptr_array *ptr_array, void *element);

void
ptr_array_clear(struct ptr_array *ptr_array,
                ptr_array_free_element_fn free_element);

// Returns -1 if element is not in ptr_array.
int
ptr_array_index_of(struct ptr_array const *ptr_array, void const *element);

bool
ptr_array_remove(struct ptr_array *ptr_array, void *element);

void
ptr_array_remove_at(struct ptr_array *ptr_array, int index, void **element_out);

// Returns NULL if ptr_array is empty.
void *
ptr_array_last_element(struct ptr_array *ptr_array);


#endif

