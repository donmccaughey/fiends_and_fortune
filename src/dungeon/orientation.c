#include "orientation.h"


enum orientation
orientation_from_direction(enum direction direction)
{
    switch (direction) {
        case direction_north: return orientation_north_to_south;
        case direction_northeast: return orientation_north_east_to_south_west;
        case direction_east: return orientation_east_to_west;
        case direction_southeast: return orientation_south_east_to_north_west;
        case direction_south: return orientation_north_to_south;
        case direction_southwest: return orientation_north_east_to_south_west;
        case direction_west: return orientation_east_to_west;
        case direction_northwest: return orientation_south_east_to_north_west;
    }
}


char const *
orientation_name(enum orientation orientation)
{
    switch (orientation) {
        case orientation_north_to_south: return "North to South";
        case orientation_east_to_west: return "East to West";
        case orientation_north_east_to_south_west: return "Northeast to Southwest";
        case orientation_south_east_to_north_west: return "Southeast to Northwest";
        default: return "(no orientation)";
    }
}
