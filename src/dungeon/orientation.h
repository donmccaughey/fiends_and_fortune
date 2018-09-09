#ifndef FNF_DUNGEON_ORIENTATION_H_INCLUDED
#define FNF_DUNGEON_ORIENTATION_H_INCLUDED


#include <background/background.h>


enum orientation {
    orientation_none=0,
    orientation_north_to_south,
    orientation_east_to_west,
    orientation_northeast_to_southwest,
    orientation_southeast_to_northwest,
};


enum orientation
orientation_from_direction(enum direction direction);

char const *
orientation_name(enum orientation orientation);


#endif
