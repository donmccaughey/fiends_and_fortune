#include "orientation.h"


enum orientation
orientation_from_direction(enum direction direction)
{
    switch (direction) {
        case direction_north: return orientation_north_to_south;
        case direction_northeast: return orientation_northeast_to_southwest;
        case direction_east: return orientation_east_to_west;
        case direction_southeast: return orientation_southeast_to_northwest;
        case direction_south: return orientation_north_to_south;
        case direction_southwest: return orientation_northeast_to_southwest;
        case direction_west: return orientation_east_to_west;
        case direction_northwest: return orientation_southeast_to_northwest;
    }
}


char const *
orientation_name(enum orientation orientation)
{
    switch (orientation) {
        case orientation_north_to_south: return "north to south";
        case orientation_east_to_west: return "east to west";
        case orientation_northeast_to_southwest: return "northeast to southwest";
        case orientation_southeast_to_northwest: return "southeast to northwest";
        default: return "(no orientation)";
    }
}
