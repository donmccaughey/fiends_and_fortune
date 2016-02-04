#include "direction.h"


enum direction
direction_90_degrees_left(enum direction direction)
{
    switch (direction) {
        case direction_north: return direction_west;
        case direction_south: return direction_east;
        case direction_east: return direction_north;
        case direction_west: return direction_south;
        default: return direction_unknown;
    }
}


enum direction
direction_90_degrees_right(enum direction direction)
{
    switch (direction) {
        case direction_north: return direction_east;
        case direction_south: return direction_west;
        case direction_east: return direction_south;
        case direction_west: return direction_north;
        default: return direction_unknown;
    }
}


char const *
direction_name(enum direction direction)
{
    switch (direction) {
        case direction_north: return "North";
        case direction_south: return "South";
        case direction_east: return "East";
        case direction_west: return "West";
        default: return "unknown";
    }
}
