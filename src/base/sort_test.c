#include <assert.h>
#include <stdint.h>
#include <base/base.h>


static int
compare_uint32(void const *first, void const *second)
{
    uint32_t const *first_int = first;
    uint32_t const *second_int = second;
    return (int)(*first_int - *second_int);
}


static void
insertion_sort_test(void)
{
    uint32_t items[] = { 9, 8, 6, 1, 4, 3, 2, 7, 5, 0 };
    size_t count = sizeof items / sizeof items[0];

    insertion_sort(items, count, sizeof(items[0]), compare_uint32);

    assert(0 == items[0]);
    assert(1 == items[1]);
    assert(2 == items[2]);
    assert(3 == items[3]);
    assert(4 == items[4]);
    assert(5 == items[5]);
    assert(6 == items[6]);
    assert(7 == items[7]);
    assert(8 == items[8]);
    assert(9 == items[9]);
}


void
sort_test(void)
{
    insertion_sort_test();
}
