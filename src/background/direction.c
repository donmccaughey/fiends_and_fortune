#include "direction.h"


enum direction
direction_90_degrees_left(enum direction direction)
{
    switch (direction) {
        case direction_north: return direction_west;
        case direction_south: return direction_east;
        case direction_east: return direction_north;
        case direction_west: return direction_south;
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
    }
}


char const *
direction_name(enum direction direction)
{
    switch (direction) {
        case direction_north: return "north";
        case direction_south: return "south";
        case direction_east: return "east";
        case direction_west: return "west";
    }
}


enum direction
direction_opposite(enum direction direction)
{
    switch (direction) {
        case direction_north: return direction_south;
        case direction_south: return direction_north;
        case direction_east: return direction_west;
        case direction_west: return direction_east;
    }
}
