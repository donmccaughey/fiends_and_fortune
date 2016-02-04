#ifndef FNF_DUNGEON_ORIENTATION_H_INCLUDED
#define FNF_DUNGEON_ORIENTATION_H_INCLUDED


#include "direction.h"


enum orientation {
    orientation_unknown=0,
    orientation_north_south,
    orientation_east_west
};


enum orientation
orientation_from_direction(enum direction direction);

char const *
orientation_name(enum orientation orientation);


#endif
