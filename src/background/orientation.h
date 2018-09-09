#ifndef FNF_BACKGROUND_ORIENTATION_H_INCLUDED
#define FNF_BACKGROUND_ORIENTATION_H_INCLUDED


#include <stdbool.h>
#include <background/direction.h>


enum orientation {
    orientation_north_to_south         =   0,
    orientation_northeast_to_southwest =  45,
    orientation_east_to_west           =  90,
    orientation_southeast_to_northwest = 135,
};


enum orientation
orientation_from_direction(enum direction direction);

inline bool
orientation_is_valid(unsigned orientation)
{
    return orientation < 180 && 0 == orientation % 45;
}

char const *
orientation_name(enum orientation orientation);


#endif
