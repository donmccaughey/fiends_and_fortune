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
}


static void
direction_name_test(void)
{
    assert(str_eq("North", direction_name(direction_north)));
    assert(str_eq("South", direction_name(direction_south)));
    assert(str_eq("East", direction_name(direction_east)));
    assert(str_eq("West", direction_name(direction_west)));
}


void
direction_test(void)
{
    direction_90_degrees_left_test();
    direction_90_degrees_right_test();
    direction_opposite_test();
    direction_name_test();
}
