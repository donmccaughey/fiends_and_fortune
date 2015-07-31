#ifndef FNF_DUNGEON_DIRECTION_H_INCLUDED
#define FNF_DUNGEON_DIRECTION_H_INCLUDED


enum direction {
    Unknown = 0,
    North,
    South,
    East,
    West,
};


char const *
direction_name(enum direction direction);


#endif
