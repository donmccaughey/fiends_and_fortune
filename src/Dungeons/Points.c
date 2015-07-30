#include "Points.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"


struct Points *addPointToPoints(struct Point point, struct Points *next)
{
    struct Points *points = malloc_or_die(sizeof(struct Points));
    points->next = next;
    points->point = point;
    return points;
}


struct Points *addPointsToPoints(struct Points *points1, struct Points *points2)
{
    struct Points *last = points1;
    while (last->next) last = last->next;
    last->next = points2;
    return points1;
}


void destroyPoints(struct Points *points)
{
    while (points) {
        struct Points *next = points->next;
        free_or_die(points);
        points = next;
    }
}


size_t pointsCount(struct Points *points)
{
    size_t count = 0;
    while (points) {
        ++count;
        points = points->next;
    }
    return count;
}
