#ifndef FNF_DUNGEON_POINT_H_INCLUDED
#define FNF_DUNGEON_POINT_H_INCLUDED


#include <stdint.h>

#include "direction.h"


struct point {
    int32_t x;
    int32_t y;
    int32_t z;
};


int
point_compare(struct point point1, struct point point2);

struct point
point_make(int32_t x, int32_t y, int32_t z);

struct point
point_rotate_90_degrees_left(struct point point, enum direction direction);

struct point
point_rotate_90_degrees_right(struct point point, enum direction direction);


#endif
