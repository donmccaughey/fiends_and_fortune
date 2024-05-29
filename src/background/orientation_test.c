#include <assert.h>
#include <background/background.h>

#include "astr.h"


void
orientation_test(void);


static void
orientation_from_direction_test(void)
{
    assert(orientation_north_to_south == orientation_from_direction(direction_north));
    assert(orientation_north_to_south == orientation_from_direction(direction_south));

    assert(orientation_northeast_to_southwest == orientation_from_direction(direction_northeast));
    assert(orientation_northeast_to_southwest == orientation_from_direction(direction_southwest));

    assert(orientation_east_to_west == orientation_from_direction(direction_east));
    assert(orientation_east_to_west == orientation_from_direction(direction_west));

    assert(orientation_southeast_to_northwest == orientation_from_direction(direction_northwest));
    assert(orientation_southeast_to_northwest == orientation_from_direction(direction_southeast));
}


static void
orientation_is_valid_test(void)
{
    assert(orientation_is_valid(orientation_north_to_south));
    assert(orientation_is_valid(orientation_northeast_to_southwest));
    assert(orientation_is_valid(orientation_east_to_west));
    assert(orientation_is_valid(orientation_southeast_to_northwest));

    assert( ! orientation_is_valid(1));
    assert( ! orientation_is_valid(15));
    assert( ! orientation_is_valid(120));
    assert( ! orientation_is_valid(180));
}


static void
orientation_name_test(void)
{
    assert(astr_eq("north to south", orientation_name(orientation_north_to_south)));
    assert(astr_eq("northeast to southwest", orientation_name(orientation_northeast_to_southwest)));
    assert(astr_eq("east to west", orientation_name(orientation_east_to_west)));
    assert(astr_eq("southeast to northwest", orientation_name(orientation_southeast_to_northwest)));
}


void
orientation_test(void)
{
    orientation_from_direction_test();
    orientation_is_valid_test();
    orientation_name_test();
}
