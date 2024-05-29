#include "point.h"

#include <assert.h>
#include <limits.h>

#include "astr.h"


struct point
point_above(struct point point)
{
    assert(point.z > INT_MIN);
    return point_make(point.x, point.y, point.z - 1);
}


char *
point_alloc_xy(struct point point)
{
    return astr_f("(%i, %i)", point.x, point.y);
}


struct point
point_below(struct point point)
{
    assert(point.z < INT_MAX);
    return point_make(point.x, point.y, point.z + 1);
}


int
point_compare(struct point point, struct point other)
{
    if (point.z != other.z) return point.z - other.z;
    if (point.y != other.y) return point.y - other.y;
    return point.x - other.x;
}


struct point
point_east(struct point point)
{
    assert(point.x < INT_MAX);
    return point_make(point.x + 1, point.y, point.z);
}


bool
point_equals(struct point point, struct point other)
{
    return 0 == point_compare(point, other);
}


struct point
point_make(int x, int y, int z)
{
    return (struct point){ .x=x, .y=y, .z=z };
}


struct point
point_move(struct point start, int distance, enum direction direction)
{
    assert(direction_is_valid(direction));
    switch (direction) {
        case direction_north:
            return point_make(start.x, start.y + distance, start.z);
        case direction_northeast:
            return point_make(start.x + distance, start.y + distance, start.z);
        case direction_east:
            return point_make(start.x + distance, start.y, start.z);
        case direction_southeast:
            return point_make(start.x + distance, start.y - distance, start.z);
        case direction_south:
            return point_make(start.x, start.y - distance, start.z);
        case direction_southwest:
            return point_make(start.x - distance, start.y - distance, start.z);
        case direction_west:
            return point_make(start.x - distance, start.y, start.z);
        case direction_northwest:
            return point_make(start.x - distance, start.y + distance, start.z);
    }
    return point_make(start.x, start.y + distance, start.z);
}


struct point
point_north(struct point point)
{
    assert(point.y < INT_MAX);
    return point_make(point.x, point.y + 1, point.z);
}


struct point
point_northeast(struct point point)
{
    assert(point.x < INT_MAX);
    assert(point.y < INT_MAX);
    return point_make(point.x + 1, point.y + 1, point.z);
}


struct point
point_northwest(struct point point)
{
    assert(point.x > INT_MIN);
    assert(point.y < INT_MAX);
    return point_make(point.x - 1, point.y + 1, point.z);
}


struct point
point_rotate_90_degrees_left(struct point point, enum direction direction)
{
    assert(direction_is_valid(direction));
    switch (direction) {
        case direction_north:
            assert(point.x > INT_MIN);
            assert(point.y > INT_MIN);
            return point_make(point.x - 1, point.y - 1, point.z);
        case direction_northeast:
            assert(point.x > INT_MIN + 1);
            return point_make(point.x - 2, point.y, point.z);
        case direction_east:
            assert(point.x > INT_MIN);
            assert(point.y < INT_MAX);
            return point_make(point.x - 1, point.y + 1, point.z);
        case direction_southeast:
            assert(point.y < INT_MAX - 1);
            return point_make(point.x, point.y + 2, point.z);
        case direction_south:
            assert(point.x < INT_MAX);
            assert(point.y < INT_MAX);
            return point_make(point.x + 1, point.y + 1, point.z);
        case direction_southwest:
            assert(point.x < INT_MAX - 1);
            return point_make(point.x + 2, point.y, point.z);
        case direction_west:
            assert(point.x < INT_MAX);
            assert(point.y > INT_MIN);
            return point_make(point.x + 1, point.y - 1, point.z);
        case direction_northwest:
            assert(point.y > INT_MIN + 1);
            return point_make(point.x, point.y - 2, point.z);
    }
    return point_make(point.x - 1, point.y - 1, point.z);
}


struct point
point_rotate_90_degrees_right(struct point point, enum direction direction)
{
    assert(direction_is_valid(direction));
    switch (direction) {
        case direction_north:
            assert(point.x < INT_MAX);
            assert(point.y > INT_MIN);
            return point_make(point.x + 1, point.y - 1, point.z);
        case direction_northeast:
            assert(point.y > INT_MIN + 1);
            return point_make(point.x, point.y - 2, point.z);
        case direction_east:
            assert(point.x > INT_MIN);
            assert(point.y > INT_MIN);
            return point_make(point.x - 1, point.y - 1, point.z);
        case direction_southeast:
            assert(point.x > INT_MIN + 1);
            return point_make(point.x - 2, point.y, point.z);
        case direction_south:
            assert(point.x > INT_MIN);
            assert(point.y < INT_MAX);
            return point_make(point.x - 1, point.y + 1, point.z);
        case direction_southwest:
            assert(point.y < INT_MAX - 1);
            return point_make(point.x, point.y + 2, point.z);
        case direction_west:
            assert(point.x < INT_MAX);
            assert(point.y < INT_MAX);
            return point_make(point.x + 1, point.y + 1, point.z);
        case direction_northwest:
            assert(point.x < INT_MAX - 1);
            return point_make(point.x + 2, point.y, point.z);
    }
    return point_make(point.x + 1, point.y - 1, point.z);
}


struct point
point_south(struct point point)
{
    assert(point.y > INT_MIN);
    return point_make(point.x, point.y - 1, point.z);
}


struct point
point_southeast(struct point point)
{
    assert(point.x < INT_MAX);
    assert(point.y > INT_MIN);
    return point_make(point.x + 1, point.y - 1, point.z);
}


struct point
point_southwest(struct point point)
{
    assert(point.x > INT_MIN);
    assert(point.y > INT_MIN);
    return point_make(point.x - 1, point.y - 1, point.z);
}


struct point
point_west(struct point point)
{
    assert(point.x > INT_MIN);
    return point_make(point.x - 1, point.y, point.z);
}
