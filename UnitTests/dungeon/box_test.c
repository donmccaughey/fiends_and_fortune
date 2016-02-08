#include "box.h"

#include <assert.h>


void
box_test(void);


static void
box_make_test(void)
{
    struct box box = box_make(point_make(1, 2, 3), size_make(4, 5, 6));
    assert(1 == box.origin.x);
    assert(2 == box.origin.y);
    assert(3 == box.origin.z);
    assert(4 == box.size.width);
    assert(5 == box.size.length);
    assert(6 == box.size.height);
}


static void
box_make_from_boxes_test(void)
{
    struct box box1 = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    struct box box2 = box_make(point_make(1, 0, 0), size_make(1, 1, 1));
    struct box box3 = box_make(point_make(3, 4, 5), size_make(1, 1, 1));
    struct box box4 = box_make(point_make(-2, -2, -2), size_make(2, 2, 2));
    
    struct box box = box_make_from_boxes(box1, box2);
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(2 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
    
    box = box_make_from_boxes(box1, box3);
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(4 == box.size.width);
    assert(5 == box.size.length);
    assert(6 == box.size.height);
    
    box = box_make_from_boxes(box3, box4);
    
    assert(-2 == box.origin.x);
    assert(-2 == box.origin.y);
    assert(-2 == box.origin.z);
    assert(6 == box.size.width);
    assert(7 == box.size.length);
    assert(8 == box.size.height);
}


static void
box_make_from_points_test(void)
{
    struct point point1 = point_make(0, 0, 0);
    struct point point2 = point_make(1, 1, 1);
    struct point point3 = point_make(3, 4, 5);
    struct point point4 = point_make(-2, -2, -2);
    
    struct box box = box_make_from_points(point1, point2);
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(1 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
    
    box = box_make_from_points(point2, point3);
    
    assert(1 == box.origin.x);
    assert(1 == box.origin.y);
    assert(1 == box.origin.z);
    assert(2 == box.size.width);
    assert(3 == box.size.length);
    assert(4 == box.size.height);
    
    box = box_make_from_points(point4, point3);
    
    assert(-2 == box.origin.x);
    assert(-2 == box.origin.y);
    assert(-2 == box.origin.z);
    assert(5 == box.size.width);
    assert(6 == box.size.length);
    assert(7 == box.size.height);
    
    box = box_make_from_points(point3, point4);
    
    assert(3 == box.origin.x);
    assert(4 == box.origin.y);
    assert(5 == box.origin.z);
    assert(-5 == box.size.width);
    assert(-6 == box.size.length);
    assert(-7 == box.size.height);
}


static void
box_normalize_test(void)
{
    struct box box1 = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    struct box box2 = box_make(point_make(0, 0, 0), size_make(-1, 1, 1));
    struct box box3 = box_make(point_make(0, 0, 0), size_make(-2, 1, 1));
    struct box box4 = box_make(point_make(-4, -4, -4), size_make(-2, -3, -4));
    
    struct box box = box_normalize(box1);
    assert(box_equals(box1, box));
    
    box = box_normalize(box2);
    assert(point_equals(point_make(0, 0, 0), box.origin));
    assert(size_equals(size_make(1, 1, 1), box.size));
    
    box = box_normalize(box3);
    assert(point_equals(point_make(-1, 0, 0), box.origin));
    assert(size_equals(size_make(2, 1, 1), box.size));
    
    box = box_normalize(box4);
    assert(point_equals(point_make(-5, -6, -7), box.origin));
    assert(size_equals(size_make(2, 3, 4), box.size));
}


static void
box_expand_test(void)
{
    struct box box = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    box = box_expand(box, size_make(0, 0, 0));
    
    assert(0 == box.origin.x);
    assert(0 == box.origin.y);
    assert(0 == box.origin.z);
    assert(1 == box.size.width);
    assert(1 == box.size.length);
    assert(1 == box.size.height);
    
    box = box_expand(box, size_make(1, 1, 1));
    
    assert(-1 == box.origin.x);
    assert(-1 == box.origin.y);
    assert(-1 == box.origin.z);
    assert(3 == box.size.width);
    assert(3 == box.size.length);
    assert(3 == box.size.height);
    
    box = box_expand(box, size_make(1, 2, -1));
    
    assert(-2 == box.origin.x);
    assert(-3 == box.origin.y);
    assert(0 == box.origin.z);
    assert(5 == box.size.width);
    assert(7 == box.size.length);
    assert(1 == box.size.height);
}


static void
box_extend_to_include_point_test(void)
{
    struct box box = box_make(point_make(0, 0, 0), size_make(1, 1, 1));
    
    box = box_extend_to_include_point(box, point_make(1, 2, 3));
    assert(point_equals(point_make(0, 0, 0), box.origin));
    assert(size_equals(size_make(2, 3, 4), box.size));
    
    box = box_extend_to_include_point(box, point_make(-3, 0, 0));
    assert(point_equals(point_make(-3, 0, 0), box.origin));
    assert(size_equals(size_make(5, 3, 4), box.size));
    
    box = box_extend_to_include_point(box, point_make(4, 4, 4));
    assert(point_equals(point_make(-3, 0, 0), box.origin));
    assert(size_equals(size_make(8, 5, 5), box.size));
    
    box = box_extend_to_include_point(box, point_make(-3, -3, -3));
    assert(point_equals(point_make(-3, -3, -3), box.origin));
    assert(size_equals(size_make(8, 8, 8), box.size));
}


static void
box_index_for_point_test(void)
{
    struct box box = box_make(point_make(0, 0, 0), size_make(2, 3, 4));
    
    assert(0 == box_index_for_point(box, point_make(0, 0, 0)));
    assert(1 == box_index_for_point(box, point_make(1, 0, 0)));
    assert(2 == box_index_for_point(box, point_make(0, 1, 0)));
    assert(3 == box_index_for_point(box, point_make(1, 1, 0)));
    assert(4 == box_index_for_point(box, point_make(0, 2, 0)));
    assert(5 == box_index_for_point(box, point_make(1, 2, 0)));
    assert(6 == box_index_for_point(box, point_make(0, 0, 1)));
    assert(7 == box_index_for_point(box, point_make(1, 0, 1)));
}


static void
box_intersects_test(void)
{
    struct box box1 = box_make(point_make(0, 0, 0), size_make(10, 10, 1));
    struct box box2 = box_make(point_make(-5, -5, 0), size_make(10, 10, 1));
    struct box box3 = box_make(point_make(5, 5, 0), size_make(10, 10, 1));
    struct box box4 = box_make(point_make(3, 3, 0), size_make(3, 3, 1));
    struct box box5 = box_make(point_make(3, 3, 1), size_make(3, 3, 1));
    struct box box6 = box_make(point_make(3, 3, -1), size_make(3, 3, 1));
    struct box box7 = box_make(point_make(3, 3, -1), size_make(3, 3, 3));
    
    assert(box_intersects(box1, box1));
    assert(box_intersects(box1, box2));
    assert(box_intersects(box1, box3));
    assert(!box_intersects(box2, box3));
    assert(box_intersects(box1, box4));
    assert(box_intersects(box2, box4));
    assert(box_intersects(box3, box4));
    assert(!box_intersects(box4, box5));
    assert(!box_intersects(box4, box6));
    assert(box_intersects(box4, box7));
    assert(box_intersects(box5, box7));
}


void box_test(void)
{
    box_make_test();
    box_make_from_boxes_test();
    box_make_from_points_test();
    box_normalize_test();
    box_expand_test();
    box_extend_to_include_point_test();
    box_index_for_point_test();
    box_intersects_test();
}
