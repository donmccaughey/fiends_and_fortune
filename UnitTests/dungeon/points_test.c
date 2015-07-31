#include <assert.h>
#include <stddef.h>

#include "points.h"


void
points_test(void);


static void
points_alloc_and_push_point_test(void)
{
    struct points *points = NULL;

    points_alloc_and_push_point(&points, point_make(1, 2, 3));
    assert(points);
    assert(1 == points->point.x);
    assert(2 == points->point.y);
    assert(3 == points->point.z);
    assert(1 == points_count(points));
    
    points_alloc_and_push_point(&points, point_make(4, 5, 6));
    assert(points);
    assert(4 == points->point.x);
    assert(5 == points->point.y);
    assert(6 == points->point.z);
    assert(2 == points_count(points));
    
    points_free(points);
}


static void
points_push_points_test(void)
{
    struct points *points1 = NULL;
    points_alloc_and_push_point(&points1, point_make(1, 2, 3));
    points_alloc_and_push_point(&points1, point_make(4, 5, 6));
    points_alloc_and_push_point(&points1, point_make(7, 8, 9));
    
    struct points *points2 = NULL;
    points_alloc_and_push_point(&points2, point_make(10, 11, 12));
    
    points_push_points(&points2, points1);
    assert(points2);
    assert(7 == points2->point.x);
    assert(8 == points2->point.y);
    assert(9 == points2->point.z);
    assert(4 == points_count(points2));
    
    points_free(points2);
}


void points_test(void)
{
    points_alloc_and_push_point_test();
    points_push_points_test();
}
