#ifndef FNF_DUNGEON_POINTS_H_INCLUDED
#define FNF_DUNGEON_POINTS_H_INCLUDED


#include <stddef.h>

#include "point.h"


struct points {
    struct points *next;
    struct point point;
};


void
points_alloc_and_push_point(struct points **points, struct point point);

void
points_push_points(struct points **points, struct points *new_points);

void
points_free(struct points *points);

size_t
points_count(struct points *points);


#endif
