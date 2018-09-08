#ifndef FNF_BACKGROUND_DIRECTION_H_INCLUDED
#define FNF_BACKGROUND_DIRECTION_H_INCLUDED


enum direction {
    direction_north = 0,
    direction_south,
    direction_east,
    direction_west,
};


enum direction
direction_90_degrees_left(enum direction direction);

enum direction
direction_90_degrees_right(enum direction direction);

enum direction
direction_opposite(enum direction direction);

char const *
direction_name(enum direction direction);


#endif
