#include "points.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"


void
points_alloc_and_push_point(struct points **points, struct point point)
{
    struct points *new_points = malloc_or_die(sizeof(struct points));
    new_points->next = *points;
    new_points->point = point;
    *points = new_points;
}


void
points_push_points(struct points **points, struct points *new_points)
{
    struct points *last = new_points;
    while (last->next) last = last->next;
    last->next = *points;
    *points = new_points;
}


void
points_free(struct points *points)
{
    while (points) {
        struct points *next = points->next;
        free_or_die(points);
        points = next;
    }
}


size_t
points_count(struct points *points)
{
    size_t count = 0;
    while (points) {
        ++count;
        points = points->next;
    }
    return count;
}
