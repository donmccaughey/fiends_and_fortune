#include <assert.h>
#include <background/background.h>
#include <base/base.h>


void
direction_test(void);


static void
direction_90_degrees_left_test(void)
{
    enum direction direction = direction_south;

    direction = direction_90_degrees_left(direction);
    assert(direction_east == direction);

    direction = direction_90_degrees_left(direction);
    assert(direction_north == direction);

    direction = direction_90_degrees_left(direction);
    assert(direction_west == direction);

    direction = direction_90_degrees_left(direction);
    assert(direction_south == direction);

    // intercardinal directions

    direction = direction_southeast;

    direction = direction_90_degrees_left(direction);
    assert(direction_northeast == direction);

    direction = direction_90_degrees_left(direction);
    assert(direction_northwest == direction);

    direction = direction_90_degrees_left(direction);
    assert(direction_southwest == direction);

    direction = direction_90_degrees_left(direction);
    assert(direction_southeast == direction);
}


static void
direction_90_degrees_right_test(void)
{
    enum direction direction = direction_south;

    direction = direction_90_degrees_right(direction);
    assert(direction_west == direction);

    direction = direction_90_degrees_right(direction);
    assert(direction_north == direction);

    direction = direction_90_degrees_right(direction);
    assert(direction_east == direction);

    direction = direction_90_degrees_right(direction);
    assert(direction_south == direction);

    // intercardinal directions

    direction = direction_southeast;

    direction = direction_90_degrees_right(direction);
    assert(direction_southwest == direction);

    direction = direction_90_degrees_right(direction);
    assert(direction_northwest == direction);

    direction = direction_90_degrees_right(direction);
    assert(direction_northeast == direction);

    direction = direction_90_degrees_right(direction);
    assert(direction_southeast == direction);
}


static void
direction_name_test(void)
{
    assert(str_eq("north", direction_name(direction_north)));
    assert(str_eq("northeast", direction_name(direction_northeast)));
    assert(str_eq("east", direction_name(direction_east)));
    assert(str_eq("southeast", direction_name(direction_southeast)));
    assert(str_eq("south", direction_name(direction_south)));
    assert(str_eq("southwest", direction_name(direction_southwest)));
    assert(str_eq("west", direction_name(direction_west)));
    assert(str_eq("northwest", direction_name(direction_northwest)));
}


static void
direction_opposite_test(void)
{
    enum direction direction = direction_south;

    direction = direction_opposite(direction);
    assert(direction_north == direction);

    direction = direction_opposite(direction);
    assert(direction_south == direction);


    direction = direction_west;

    direction = direction_opposite(direction);
    assert(direction_east == direction);

    direction = direction_opposite(direction);
    assert(direction_west == direction);

    // intercardinal directions

    direction = direction_southeast;

    direction = direction_opposite(direction);
    assert(direction_northwest == direction);

    direction = direction_opposite(direction);
    assert(direction_southeast == direction);


    direction = direction_southwest;

    direction = direction_opposite(direction);
    assert(direction_northeast == direction);

    direction = direction_opposite(direction);
    assert(direction_southwest == direction);
}


static void
direction_random_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(0);

    assert(direction_north == direction_random(rnd));
    assert(direction_northeast == direction_random(rnd));
    assert(direction_east == direction_random(rnd));
    assert(direction_southeast == direction_random(rnd));
    assert(direction_south == direction_random(rnd));
    assert(direction_southwest == direction_random(rnd));
    assert(direction_west == direction_random(rnd));
    assert(direction_northwest == direction_random(rnd));
    assert(direction_north == direction_random(rnd));

    rnd_free(rnd);
}


void
direction_test(void)
{
    direction_90_degrees_left_test();
    direction_90_degrees_right_test();
    direction_name_test();
    direction_opposite_test();
    direction_random_test();
}
