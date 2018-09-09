#ifndef FNF_BACKGROUND_DIRECTION_H_INCLUDED
#define FNF_BACKGROUND_DIRECTION_H_INCLUDED


#include <stdbool.h>


struct rnd;


enum direction {
    direction_north     =   0,
    direction_northeast =  45,
    direction_east      =  90,
    direction_southeast = 135,
    direction_south     = 180,
    direction_southwest = 225,
    direction_west      = 270,
    direction_northwest = 315,
};


enum direction
direction_90_degrees_left(enum direction direction);

enum direction
direction_90_degrees_right(enum direction direction);

inline bool
direction_is_valid(unsigned direction)
{
    return direction < 360 && 0 == direction % 45;
}

char const *
direction_name(enum direction direction);

enum direction
direction_opposite(enum direction direction);

enum direction
direction_random(struct rnd *rnd);


#endif
