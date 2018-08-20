#include <assert.h>
#include <base/base.h>


void
int_test(void);


static void
fill_test(void)
{
    int values[5];

    fill(values, 5);

    assert(0 == values[0]);
    assert(1 == values[1]);
    assert(2 == values[2]);
    assert(3 == values[3]);
    assert(4 == values[4]);
}


static void
fill_shuffled_test(void)
{
    int values[5];
    struct rnd *rnd = rnd_alloc_fake_ascending(1);

    fill_shuffled(rnd, values, 5);

    assert(1 == values[0]);
    assert(3 == values[1]);
    assert(2 == values[2]);
    assert(0 == values[3]);
    assert(4 == values[4]);

    rnd_free(rnd);
}


static void
max_test(void)
{
    assert(5 == max(5, 5));

    assert(5 == max(1, 5));
    assert(5 == max(5, 1));

    assert(5 == max(-1, 5));
    assert(5 == max(5, -1));
}


static void
min_test(void)
{
    assert(5 == min(5, 5));

    assert(1 == min(1, 5));
    assert(1 == min(5, 1));

    assert(-1 == min(-1, 5));
    assert(-1 == min(5, -1));
}


static void
swap_test(void)
{
    int a = 1;
    int b = 5;

    assert(1 == a);
    assert(5 == b);

    swap(&a, &b);

    assert(5 == a);
    assert(1 == b);

    swap(&b, &a);

    assert(1 == a);
    assert(5 == b);
}

void
int_test(void)
{
    fill_test();
    fill_shuffled_test();
    max_test();
    min_test();
    swap_test();
}
