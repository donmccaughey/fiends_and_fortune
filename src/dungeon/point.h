#ifndef FNF_DUNGEON_POINT_H_INCLUDED
#define FNF_DUNGEON_POINT_H_INCLUDED


#include <stdbool.h>

#include "direction.h"


struct point {
    int x;
    int y;
    int z;
};


int
point_compare(struct point point, struct point other);

bool
point_equals(struct point point, struct point other);

struct point
point_make(int x, int y, int z);

struct point
point_move(struct point start, int steps, enum direction direction);

struct point
point_rotate_90_degrees_left(struct point point, enum direction direction);

struct point
point_rotate_90_degrees_right(struct point point, enum direction direction);


#endif
