#include "direction.h"

#include <assert.h>


enum direction
direction_90_degrees_left(enum direction direction)
{
    assert(direction >= 0 && direction <= 360);

    return (direction + 270) % 360;
}


enum direction
direction_90_degrees_right(enum direction direction)
{
    assert(direction >= 0 && direction <= 360);

    return (direction + 90) % 360;
}


char const *
direction_name(enum direction direction)
{
    switch (direction) {
        case direction_north: return "north";
        case direction_northeast: return "northeast";
        case direction_east: return "east";
        case direction_southeast: return "southeast";
        case direction_south: return "south";
        case direction_southwest: return "southwest";
        case direction_west: return "west";
        case direction_northwest: return "northwest";
    }
}


enum direction
direction_opposite(enum direction direction)
{
    assert(direction >= 0 && direction <= 360);

    return (direction + 180) % 360;
}
