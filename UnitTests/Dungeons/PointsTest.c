#include <assert.h>
#include <stddef.h>

#include "point.h"
#include "Points.h"


void pointsTest(void);
static void addPointToPointsTest(void);
static void addPointsToPointsTest(void);


void pointsTest(void)
{
    addPointToPointsTest();
    addPointsToPointsTest();
}


static void addPointToPointsTest(void)
{
    struct Points *points = addPointToPoints(point_make(1, 2, 3), NULL);
    assert(points);
    assert(1 == points->point.x);
    assert(2 == points->point.y);
    assert(3 == points->point.z);
    assert(1 == pointsCount(points));
    
    points = addPointToPoints(point_make(4, 5, 6), points);
    assert(points);
    assert(4 == points->point.x);
    assert(5 == points->point.y);
    assert(6 == points->point.z);
    assert(2 == pointsCount(points));
    
    destroyPoints(points);
}


static void addPointsToPointsTest(void)
{
    struct Points *points1 = addPointToPoints(point_make(1, 2, 3), NULL);
    points1 = addPointToPoints(point_make(4, 5, 6), points1);
    points1 = addPointToPoints(point_make(7, 8, 9), points1);
    
    struct Points *points2 = addPointToPoints(point_make(10, 11, 12), NULL);
    
    struct Points *points = addPointsToPoints(points1, points2);
    assert(points);
    assert(7 == points->point.x);
    assert(8 == points->point.y);
    assert(9 == points->point.z);
    assert(4 == pointsCount(points));
    
    destroyPoints(points);
}
