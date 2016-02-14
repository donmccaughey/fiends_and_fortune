#ifndef FNF_DUNGEON_ORIENTATION_H_INCLUDED
#define FNF_DUNGEON_ORIENTATION_H_INCLUDED


#include "direction.h"


enum orientation {
    orientation_none=0,
    orientation_north_to_south,
    orientation_east_to_west,
    orientation_north_east_to_south_west,
    orientation_south_east_to_north_west,
};


enum orientation
orientation_from_direction(enum direction direction);

char const *
orientation_name(enum orientation orientation);


#endif
