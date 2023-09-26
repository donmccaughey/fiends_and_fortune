#include <assert.h>
#include <base/base.h>
#include <math.h>
#include <stdint.h>


static void
array_size_test(void)
{
    bool overflow;
    assert(256 == array_size(32, 8, &overflow));
    assert( ! overflow);

    assert(SIZE_MAX == array_size(SIZE_MAX, 1, &overflow));
    assert( ! overflow);

    assert(SIZE_MAX == array_size(1, SIZE_MAX, &overflow));
    assert( ! overflow);

    size_t sqrt_size_max = (size_t)sqrt((double)SIZE_MAX);

    // fast path overflow check
    size_t expected = 2 * (sqrt_size_max - 1);
    assert(expected == array_size(sqrt_size_max - 1, 2, &overflow));
    assert( ! overflow);

    expected = 2 * (sqrt_size_max - 1);
    assert(expected == array_size(2, sqrt_size_max - 1, &overflow));
    assert( ! overflow);

    expected = (sqrt_size_max - 1) * (sqrt_size_max - 1);
    assert(expected == array_size(sqrt_size_max - 1, sqrt_size_max - 1, &overflow));
    assert( ! overflow);

    // slow path overflow check
    expected = 2 * sqrt_size_max;
    assert(expected == array_size(sqrt_size_max, 2, &overflow));
    assert( ! overflow);

    expected = 2 * sqrt_size_max;
    assert(expected == array_size(2, sqrt_size_max, &overflow));
    assert( ! overflow);

    assert(SIZE_MAX == array_size(sqrt_size_max, sqrt_size_max, &overflow));
    assert(overflow);

    // test NULL overflow flag
    assert(256 == array_size(32, 8, NULL));
    assert(SIZE_MAX == array_size(SIZE_MAX, 2, NULL));
}


#ifndef HAS_REALLOCARRAY
static void
reallocarray_function_test(void)
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
    assert(1 == array[0]);
    assert(5 == array[4]);

    errno = 0;
    array = reallocarray(array, 0, sizeof(int));
    // array may be NULL or a valid pointer
    assert(0 == errno);
    free(array);

    errno = 0;
    array = reallocarray(NULL, 0, sizeof(int));
    // array may be NULL or a valid pointer
    assert(0 == errno);
    free(array);

    array = reallocarray(NULL, 10, sizeof(int));
    assert(array);
    free(array);

    errno = 0;
    array = reallocarray(NULL, SIZE_MAX / 2, 4);
    assert( ! array);
    assert(ENOMEM == errno);
}
#endif


void
reallocarray_test(void)
{
    array_size_test();
#ifndef HAS_REALLOCARRAY
    reallocarray_function_test();
#endif
}
