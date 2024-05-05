#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <base/base.h>

#include "xstring.h"


void
ptr_array_test(void);


static void
ptr_array_alloc_test(void)
{
    struct ptr_array *ptr_array = ptr_array_alloc();

    assert(ptr_array->elements);
    assert(!ptr_array->count);
    assert(ptr_array->capacity > 0);
    assert(!ptr_array_last_element(ptr_array));

    bool removed = ptr_array_remove(ptr_array, "foo");
    assert(!removed);

    ptr_array_free(ptr_array);
}


static void
ptr_array_add_remove_test(void)
{
    struct ptr_array *ptr_array = ptr_array_alloc();
    
    char foo[] = "foo";
    ptr_array_add(ptr_array, foo);
    assert(1 == ptr_array->count);
    assert(foo == ptr_array_last_element(ptr_array));

    char bar[] = "bar";
    ptr_array_add(ptr_array, bar);
    assert(2 == ptr_array->count);
    assert(bar == ptr_array_last_element(ptr_array));

    char baz[] = "baz";
    ptr_array_add(ptr_array, baz);
    assert(3 == ptr_array->count);
    assert(baz == ptr_array_last_element(ptr_array));

    // remove last item
    bool removed = ptr_array_remove(ptr_array, baz);
    assert(removed);
    assert(2 == ptr_array->count);
    assert(bar == ptr_array_last_element(ptr_array));

    // remove first item
    removed = ptr_array_remove(ptr_array, foo);
    assert(removed);
    assert(1 == ptr_array->count);
    assert(bar == ptr_array_last_element(ptr_array));

    // remove item not in array
    removed = ptr_array_remove(ptr_array, foo);
    assert(!removed);
    assert(1 == ptr_array->count);
    assert(bar == ptr_array_last_element(ptr_array));

    // remove sole item
    removed = ptr_array_remove(ptr_array, bar);
    assert(removed);
    assert(!ptr_array->count);
    assert(!ptr_array_last_element(ptr_array));

    ptr_array_free(ptr_array);
}


static void
ptr_array_clear_test(void)
{
    struct ptr_array *ptr_array = ptr_array_alloc();
    int test_count = 32;

    for (int i = 0; i < test_count; ++i) {
        char *element = strdup("foo");
        ptr_array_add(ptr_array, element);
    }
    assert(test_count == ptr_array->count);

    ptr_array_clear(ptr_array, (ptr_array_free_element_fn)free);

    assert(!ptr_array->count);

    ptr_array_free(ptr_array);
}


static void
ptr_array_index_of_test(void)
{
    struct ptr_array *ptr_array = ptr_array_alloc();

    char foo[] = "foo";
    ptr_array_add(ptr_array, foo);
    char bar[] = "bar";
    ptr_array_add(ptr_array, bar);
    char baz[] = "baz";
    ptr_array_add(ptr_array, baz);

    int index = ptr_array_index_of(ptr_array, foo);
    assert(0 == index);

    index = ptr_array_index_of(ptr_array, bar);
    assert(1 == index);

    index = ptr_array_index_of(ptr_array, baz);
    assert(2 == index);

    index = ptr_array_index_of(ptr_array, "not in array");
    assert(-1 == index);

    ptr_array_free(ptr_array);
}


static void
ptr_array_remove_at_test(void)
{
    struct ptr_array *ptr_array = ptr_array_alloc();
    char foo[] = "foo";
    ptr_array_add(ptr_array, foo);
    char bar[] = "bar";
    ptr_array_add(ptr_array, bar);
    char baz[] = "baz";
    ptr_array_add(ptr_array, baz);

    ptr_array_remove_at(ptr_array, 0, NULL);
    assert(2 == ptr_array->count);
    assert(bar == ptr_array->elements[0]);

    void *element;
    ptr_array_remove_at(ptr_array, 1, &element);
    assert(baz == element);
    assert(1 == ptr_array->count);
    assert(bar == ptr_array->elements[0]);

    ptr_array_remove_at(ptr_array, 0, &element);
    assert(bar == element);
    assert(!ptr_array->count);

    ptr_array_free(ptr_array);
}


static void
ptr_array_increase_capacity_test(void)
{
    struct ptr_array *ptr_array = ptr_array_alloc();
    int initial_capacity = ptr_array->capacity;
    int test_count = 1024;

    for (int i = 0; i < test_count; ++i) {
        char *element = xstrdup("foo");
        assert(element);
        ptr_array_add(ptr_array, element);
        assert(ptr_array->elements[i] == element);
    }
    assert(ptr_array->capacity > initial_capacity);

    srand(1777);
    while (ptr_array->count) {
        int i = rand() % ptr_array->count;
        void *expected = ptr_array->elements[i];
        void *element;
        ptr_array_remove_at(ptr_array, i, &element);
        assert(expected == element);
        free(element);
    }

    ptr_array_free(ptr_array);
}


void
ptr_array_test(void)
{
    ptr_array_alloc_test();
    ptr_array_add_remove_test();
    ptr_array_clear_test();
    ptr_array_index_of_test();
    ptr_array_remove_at_test();
    ptr_array_increase_capacity_test();
}

