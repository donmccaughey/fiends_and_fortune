#ifndef DUNGEONS_POINTS_H_INCLUDED
#define DUNGEONS_POINTS_H_INCLUDED


#include <stddef.h>
#include "Point.h"


struct Points {
    struct Points *next;
    struct Point point;
};


struct Points *addPointToPoints(struct Point point, struct Points *next);

struct Points *addPointsToPoints(struct Points *points1, struct Points *points2);

void destroyPoints(struct Points *points);

size_t pointsCount(struct Points *points);


#endif /* #ifndef DUNGEONS_POINTS_H_INCLUDED */
