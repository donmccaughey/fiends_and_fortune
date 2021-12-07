#include "direction.h"

#include <assert.h>
#include <base/base.h>


enum direction
direction_90_degrees_left(enum direction direction)
{
    assert(direction_is_valid(direction));
    return (direction + 270) % 360;
}


enum direction
direction_90_degrees_right(enum direction direction)
{
    assert(direction_is_valid(direction));
    return (direction + 90) % 360;
}


extern inline bool
direction_is_valid(unsigned direction);


char const *
direction_name(enum direction direction)
{
    assert(direction_is_valid(direction));
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
    return "north";
}


enum direction
direction_opposite(enum direction direction)
{
    assert(direction_is_valid(direction));
    return (direction + 180) % 360;
}


enum direction
direction_random(struct rnd *rnd)
{
    return rnd_next_uniform_value(rnd, 8) * 45;
}
