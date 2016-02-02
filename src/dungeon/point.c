#include "point.h"

#include "common/fail.h"


int
point_compare(struct point point1, struct point point2)
{
    if (point1.z != point2.z) return point1.z - point2.z;
    if (point1.y != point2.y) return point1.y - point2.y;
    return point1.x - point2.x;
}


struct point
point_make(int32_t x, int32_t y, int32_t z)
{
    return (struct point){ .x=x, .y=y, .z=z };
}


struct point
point_move(struct point start, int32_t steps, enum direction direction)
{
    switch (direction) {
        case North: return point_make(start.x, start.y + steps, start.z);
        case South: return point_make(start.x, start.y - steps, start.z);
        case East: return point_make(start.x + steps, start.y, start.z);
        case West: return point_make(start.x - steps, start.y, start.z);
        default: fail("Unrecognized direction %i", direction); break;
    }
    return start;
}


struct point
point_rotate_90_degrees_left(struct point point, enum direction direction)
{
    switch (direction) {
        case North:
            return point_make(point.x - 1, point.y - 1, point.z);
        case South:
            return point_make(point.x + 1, point.y + 1, point.z);
        case East:
            return point_make(point.x - 1, point.y + 1, point.z);
        case West:
            return point_make(point.x + 1, point.y - 1, point.z);
        default:
            return point;
    }
}


struct point
point_rotate_90_degrees_right(struct point point, enum direction direction)
{
    switch (direction) {
        case North:
            return point_make(point.x + 1, point.y - 1, point.z);
        case South:
            return point_make(point.x - 1, point.y + 1, point.z);
        case East:
            return point_make(point.x - 1, point.y - 1, point.z);
        case West:
            return point_make(point.x + 1, point.y + 1, point.z);
        default:
            return point;
    }
}
