#ifndef FNF_DUNGEON_DIRECTION_H_INCLUDED
#define FNF_DUNGEON_DIRECTION_H_INCLUDED


enum direction {
    direction_unknown=0,
    direction_north,
    direction_south,
    direction_east,
    direction_west,
};


enum direction
direction_90_degrees_left(enum direction direction);

enum direction
direction_90_degrees_right(enum direction direction);

char const *
direction_name(enum direction direction);


#endif
