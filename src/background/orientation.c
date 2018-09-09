#include "orientation.h"

#include <assert.h>


enum orientation
orientation_from_direction(enum direction direction)
{
    assert(direction_is_valid(direction));
    return direction % 180;
}


extern inline bool
orientation_is_valid(unsigned orientation);


char const *
orientation_name(enum orientation orientation)
{
    assert(orientation_is_valid(orientation));
    switch (orientation) {
        case orientation_north_to_south: return "north to south";
        case orientation_northeast_to_southwest: return "northeast to southwest";
        case orientation_east_to_west: return "east to west";
        case orientation_southeast_to_northwest: return "southeast to northwest";
    }
}
