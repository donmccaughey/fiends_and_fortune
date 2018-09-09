#include <assert.h>
#include <background/background.h>
#include <base/base.h>
#include <dungeon/dungeon.h>


void
orientation_test(void);


static void
orientation_from_direction_test(void)
{
    assert(orientation_north_to_south == orientation_from_direction(direction_north));
    assert(orientation_north_to_south == orientation_from_direction(direction_south));

    assert(orientation_east_to_west == orientation_from_direction(direction_east));
    assert(orientation_east_to_west == orientation_from_direction(direction_west));

    assert(orientation_north_east_to_south_west == orientation_from_direction(direction_northeast));
    assert(orientation_north_east_to_south_west == orientation_from_direction(direction_southwest));

    assert(orientation_south_east_to_north_west == orientation_from_direction(direction_northwest));
    assert(orientation_south_east_to_north_west == orientation_from_direction(direction_southeast));
}


static void
orientation_name_test(void)
{
    assert(str_eq("North to South", orientation_name(orientation_north_to_south)));
    assert(str_eq("East to West", orientation_name(orientation_east_to_west)));
    assert(str_eq("Northeast to Southwest", orientation_name(orientation_north_east_to_south_west)));
    assert(str_eq("Southeast to Northwest", orientation_name(orientation_south_east_to_north_west)));
}


void
orientation_test(void)
{
    orientation_from_direction_test();
    orientation_name_test();
}
