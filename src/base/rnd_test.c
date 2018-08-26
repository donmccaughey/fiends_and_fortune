#include <assert.h>
#include <base/base.h>


void
rnd_test(void);


static void
global_rnd_test(void)
{
    for (int i = 0; i < 1000; ++i) {
        uint32_t value = rnd_next_uniform_value(global_rnd, 100);
        assert(value < 100);
    }
}


static void
rnd_alloc_test(void)
{
    struct rnd *rnd = rnd_alloc();
    for (int i = 0; i < 1000; ++i) {
        uint32_t value = rnd_next_uniform_value(rnd, 100);
        assert(value < 100);
    }
    rnd_free(rnd);
}


static void
rnd_alloc_fake_ascending_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(3);

    uint32_t value = rnd_next_value(rnd);
    assert(3 == value);

    value = rnd_next_value(rnd);
    assert(4 == value);

    value = rnd_next_value(rnd);
    assert(5 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_ascending_range_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(2);

    uint32_t value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(4 == value);

    value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(5 == value);

    value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(2 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_fixed_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(3);

    uint32_t value = rnd_next_value(rnd);
    assert(3 == value);

    value = rnd_next_value(rnd);
    assert(3 == value);

    value = rnd_next_value(rnd);
    assert(3 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_fixed_range_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(2);

    uint32_t value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(4 == value);

    value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(4 == value);

    value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(4 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_max_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();

    uint32_t value = rnd_next_value(rnd);
    assert(UINT32_MAX == value);

    value = rnd_next_value(rnd);
    assert(UINT32_MAX == value);

    value = rnd_next_value(rnd);
    assert(UINT32_MAX == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_median_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();

    uint32_t value = rnd_next_value(rnd);
    assert(0x7fffffff == value);

    value = rnd_next_value(rnd);
    assert(0x7fffffff == value);

    value = rnd_next_value(rnd);
    assert(0x7fffffff == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_median_range_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();

    uint32_t value = rnd_next_uniform_value_in_range(rnd, 1, 4);
    assert(3 == value);

    value = rnd_next_uniform_value_in_range(rnd, 1, 5);
    assert(3 == value);

    value = rnd_next_uniform_value_in_range(rnd, 1, 6);
    assert(4 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_fake_min_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();

    uint32_t value = rnd_next_value(rnd);
    assert(0 == value);

    value = rnd_next_value(rnd);
    assert(0 == value);

    value = rnd_next_value(rnd);
    assert(0 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_jrand48_test(void)
{
    struct rnd *rnd = rnd_alloc_jrand48((unsigned short[]){ 2, 3, 5 });

    uint32_t value = rnd_next_value(rnd);
    assert(485716256 == value);

    value = rnd_next_value(rnd);
    assert(3350141992 == value);

    value = rnd_next_value(rnd);
    assert(3263372418 == value);

    rnd_free(rnd);
}


static void
rnd_alloc_jrand48_range_test(void)
{
    struct rnd *rnd = rnd_alloc_jrand48((unsigned short[]){ 2, 3, 5 });

    uint32_t value = rnd_next_uniform_value_in_range(rnd, 1, 99);
    assert(81 == value);

    value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(2 == value);

    value = rnd_next_uniform_value_in_range(rnd, 2, 5);
    assert(4 == value);

    rnd_free(rnd);
}


static void
rnd_next_uniform_value_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(8);

    // small upper bound is always zero

    uint32_t value = rnd_next_uniform_value(rnd, 1);
    assert(0 == value);

    value = rnd_next_uniform_value(rnd, 0);
    assert(0 == value);

    // normal upper bound increments counter

    value = rnd_next_uniform_value(rnd, 10);
    assert(8 == value);

    value = rnd_next_uniform_value(rnd, 10);
    assert(9 == value);

    value = rnd_next_uniform_value(rnd, 10);
    assert(0 == value);

    rnd_free(rnd);
}


static void
rnd_next_uniform_value_in_range_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(3);

    // small upper bound is always lower bound

    uint32_t value = rnd_next_uniform_value_in_range(rnd, 5, 4);
    assert(5 == value);

    value = rnd_next_uniform_value_in_range(rnd, 5, 5);
    assert(5 == value);

    // normal upper bound increments counter

    value = rnd_next_uniform_value_in_range(rnd, 5, 9);
    assert(8 == value);

    value = rnd_next_uniform_value_in_range(rnd, 5, 9);
    assert(9 == value);

    value = rnd_next_uniform_value_in_range(rnd, 5, 9);
    assert(5 == value);

    rnd_free(rnd);
}


static void
rnd_shuffle_test(void)
{
    int numbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    struct rnd *rnd = rnd_alloc_fake_ascending(0);

    rnd_shuffle(rnd, numbers, 10, sizeof(int));

    assert(0 == numbers[0]);
    assert(2 == numbers[1]);
    assert(4 == numbers[2]);
    assert(6 == numbers[3]);
    assert(8 == numbers[4]);
    assert(5 == numbers[5]);
    assert(1 == numbers[6]);
    assert(3 == numbers[7]);
    assert(7 == numbers[8]);
    assert(9 == numbers[9]);

    rnd_free(rnd);
}


void
rnd_test(void)
{
    global_rnd_test();
    rnd_alloc_test();
    rnd_alloc_fake_ascending_test();
    rnd_alloc_fake_ascending_range_test();
    rnd_alloc_fake_fixed_test();
    rnd_alloc_fake_fixed_range_test();
    rnd_alloc_fake_max_test();
    rnd_alloc_fake_median_test();
    rnd_alloc_fake_median_range_test();
    rnd_alloc_fake_min_test();
    rnd_alloc_jrand48_test();
    rnd_alloc_jrand48_range_test();
    rnd_next_uniform_value_test();
    rnd_next_uniform_value_in_range_test();
    rnd_shuffle_test();
}
