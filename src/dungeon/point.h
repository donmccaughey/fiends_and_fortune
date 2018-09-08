#ifndef FNF_DUNGEON_POINT_H_INCLUDED
#define FNF_DUNGEON_POINT_H_INCLUDED


#include <stdbool.h>
#include <background/background.h>


struct point {
    int x;
    int y;
    int z;
};


struct point
point_make(int x, int y, int z);

int
point_compare(struct point point, struct point other);

bool
point_equals(struct point point, struct point other);

struct point
point_move(struct point start, int distance, enum direction direction);

struct point
point_above(struct point point);

struct point
point_below(struct point point);

struct point
point_north(struct point point);

struct point
point_south(struct point point);

struct point
point_east(struct point point);

struct point
point_west(struct point point);

struct point
point_north_east(struct point point);

struct point
point_north_west(struct point point);

struct point
point_south_east(struct point point);

struct point
point_south_west(struct point point);

struct point
point_rotate_90_degrees_left(struct point point, enum direction direction);

struct point
point_rotate_90_degrees_right(struct point point, enum direction direction);

char *
point_alloc_xy(struct point point);


#endif
