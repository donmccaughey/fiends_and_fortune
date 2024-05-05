#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "xmalloc.h"


static void
arraysize_tests(void)
{
    bool overflow;
    assert(256 == arraysize(32, 8, &overflow));
    assert(!overflow);

    assert(SIZE_MAX == arraysize(SIZE_MAX, 1, &overflow));
    assert(!overflow);

    assert(SIZE_MAX == arraysize(1, SIZE_MAX, &overflow));
    assert(!overflow);

    assert(0 == arraysize(0, 8, &overflow));
    assert(!overflow);

    assert(0 == arraysize(32, 0, &overflow));
    assert(!overflow);

    size_t sqrt_size_max = (size_t)sqrt((double)SIZE_MAX);

    // fast path overflow check
    size_t expected = 2 * (sqrt_size_max - 1);
    assert(expected == arraysize(sqrt_size_max - 1, 2, &overflow));
    assert(!overflow);

    expected = 2 * (sqrt_size_max - 1);
    assert(expected == arraysize(2, sqrt_size_max - 1, &overflow));
    assert(!overflow);

    expected = (sqrt_size_max - 1) * (sqrt_size_max - 1);
    assert(expected == arraysize(sqrt_size_max - 1, sqrt_size_max - 1, &overflow));
    assert(!overflow);

    // slow path overflow check
    expected = 2 * sqrt_size_max;
    assert(expected == arraysize(sqrt_size_max, 2, &overflow));
    assert(!overflow);

    expected = 2 * sqrt_size_max;
    assert(expected == arraysize(2, sqrt_size_max, &overflow));
    assert(!overflow);

    assert(SIZE_MAX == arraysize(sqrt_size_max, sqrt_size_max, &overflow));
    assert(overflow);

    // test NULL overflow flag
    assert(256 == arraysize(32, 8, NULL));
    assert(SIZE_MAX == arraysize(SIZE_MAX, 2, NULL));
}


#ifdef XMALLOC_NEEDS_REALLOCARRAY
static void
reallocarray_tests(void)
{
    int *array = calloc(10, sizeof(int));
    assert(array);
    for (int i = 0; i < 10; ++i) {
        array[i] = i + 1;
    }
    assert(1 == array[0]);
    assert(10 == array[9]);

    array = reallocarray(array, 20, sizeof(int));
    assert(array);
    assert(1 == array[0]);
    assert(10 == array[9]);

    array = reallocarray(array, 5, sizeof(int));
    assert(array);
    assert(1 == array[0]);
    assert(5 == array[4]);

    errno = 0;
    array = reallocarray(array, 0, sizeof(int));
    // array may be NULL or a valid pointer
    assert(!errno);
    free(array);

    errno = 0;
    array = reallocarray(NULL, 0, sizeof(int));
    // array may be NULL or a valid pointer
    assert(!errno);
    free(array);

    array = reallocarray(NULL, 10, sizeof(int));
    assert(array);
    free(array);

    errno = 0;
    array = reallocarray(NULL, SIZE_MAX / 2, 4);
    assert(!array);
    assert(ENOMEM == errno);
}
#endif


#ifdef XMALLOC_NEEDS_RECALLOCARRAY
static void
recallocarray_tests(void)
{
    int *array = calloc(10, sizeof(int));
    assert(array);
    for (int i = 0; i < 10; ++i) {
        array[i] = i + 1;
    }
    assert(1 == array[0]);
    assert(10 == array[9]);

    array = recallocarray(array, 10, 20, sizeof(int));
    assert(array);
    assert(1 == array[0]);
    assert(10 == array[9]);
    assert(0 == array[10]);
    assert(0 == array[19]);

    array = recallocarray(array, 20, 5, sizeof(int));
    assert(array);
    assert(1 == array[0]);
    assert(5 == array[4]);

    errno = 0;
    array = recallocarray(array, 5, 0, sizeof(int));
    // array may be NULL or a valid pointer
    assert(!errno);
    free(array);

    errno = 0;
    array = recallocarray(NULL, 0, 0, sizeof(int));
    // array may be NULL or a valid pointer
    assert(!errno);
    free(array);

    errno = 0;
    array = recallocarray(NULL, 0, 10, sizeof(int));
    assert(!errno);
    assert(0 == array[0]);
    assert(0 == array[9]);
    free(array);
}
#endif


static void
xcalloc_test(void)
{
    int *memory = xcalloc(100, sizeof(int));
    assert(memory);
    assert(0 == memory[0]);
    free(memory);

    memory = xcalloc(0, sizeof(int));
    assert(memory);
    free(memory);
}


static void
xmalloc_function_test(void)
{
    int *memory = xmalloc(100 * sizeof(int));
    assert(memory);
    free(memory);

    memory = xmalloc(0 * sizeof(int));
    assert(memory);
    free(memory);
}


static void
xrealloc_test(void)
{
    int *memory = xmalloc(10 * sizeof(int));
    for (int i = 0; i < 10; ++i) {
        memory[i] = i + 1;
    }
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xrealloc(memory, 20 * sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xrealloc(memory, 5 * sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(5 == memory[4]);

    memory = xrealloc(memory, 0 * sizeof(int));
    assert(memory);

    free(memory);

    memory = xrealloc(NULL, 10 * sizeof(int));
    assert(memory);
    free(memory);
}


static void
xreallocarray_test(void)
{
    int *memory = xmalloc(10 * sizeof(int));
    for (int i = 0; i < 10; ++i) {
        memory[i] = i + 1;
    }
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xreallocarray(memory, 20, sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xreallocarray(memory, 5, sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(5 == memory[4]);

    memory = xreallocarray(memory, 0, sizeof(int));
    assert(memory);

    memory = xreallocarray(memory, 10, 0);
    assert(memory);

    free(memory);

    memory = xreallocarray(NULL, 10, sizeof(int));
    assert(memory);
    free(memory);
}


static void
xrecallocarray_test(void)
{
    int *memory = xmalloc(10 * sizeof(int));
    for (int i = 0; i < 10; ++i) {
        memory[i] = i + 1;
    }
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);

    memory = xrecallocarray(memory, 10, 20, sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(10 == memory[9]);
    assert(0 == memory[10]);
    assert(0 == memory[19]);

    memory = xrecallocarray(memory, 10, 5, sizeof(int));
    assert(memory);
    assert(1 == memory[0]);
    assert(5 == memory[4]);

    memory = xrecallocarray(memory, 5, 0, sizeof(int));
    assert(memory);

    memory = xrecallocarray(memory, 0, 10, sizeof(int));
    assert(memory);
    assert(0 == memory[0]);
    assert(0 == memory[9]);

    free(memory);

    memory = xrecallocarray(NULL, 0, 10, sizeof(int));
    assert(memory);
    free(memory);
}


int
main(int argc, char *argv[])
{
    arraysize_tests();
    #ifdef XMALLOC_NEEDS_REALLOCARRAY
    reallocarray_tests();
    #endif
    #ifdef XMALLOC_NEEDS_RECALLOCARRAY
    recallocarray_tests();
    #endif
    xcalloc_test();
    xmalloc_function_test();
    xrealloc_test();
    xreallocarray_test();
    xrecallocarray_test();
    return EXIT_SUCCESS;
}
