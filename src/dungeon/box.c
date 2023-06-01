#include "box.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <base/base.h>


static inline void
normalize(int *box_point, int *box_size)
{
    if (*box_size < 0) {
        *box_point = *box_point + *box_size + 1;
        *box_size = -*box_size;
    }
}


static inline struct point
point_max(struct point point, struct point other)
{
    return point_make(max_int(point.x, other.x),
                      max_int(point.y, other.y),
                      max_int(point.z, other.z));
}


static inline struct point
point_min(struct point point, struct point other)
{
    return point_make(min_int(point.x, other.x),
                      min_int(point.y, other.y),
                      min_int(point.z, other.z));
}


static void
extend_to_include(int *box_point, int *box_size, int point);


int
box_area(struct box box)
{
    assert(  (float)abs(box.size.width)
           * (float)abs(box.size.length)
           <= (float)INT_MAX);
    return abs(box.size.width) * abs(box.size.length);
}


bool
box_contains_box(struct box box, struct box other)
{
    struct point box_end = box_end_point(box);
    struct point other_end = box_end_point(other);

    return other.origin.x >= box.origin.x
        && other_end.x <= box_end.x
        && other.origin.y >= box.origin.y
        && other_end.y <= box_end.y
        && other.origin.z >= box.origin.z
        && other_end.z <= box_end.z;
}


bool
box_contains_point(struct box box, struct point point)
{
    struct point end = box_end_point(box);
    return point.z >= box.origin.z && point.z < end.z
        && point.y >= box.origin.y && point.y < end.y
        && point.x >= box.origin.x && point.x < end.x;
}


bool
box_equals(struct box box, struct box other)
{
    return point_equals(box.origin, other.origin)
        && size_equals(box.size, other.size);
}


struct point
box_end_point(struct box box)
{
    return point_make(box.origin.x + box.size.width,
                      box.origin.y + box.size.length,
                      box.origin.z + box.size.height);
}


struct box
box_expand(struct box box, struct size size)
{
    struct point new_origin = point_make(box.origin.x - size.width,
                                         box.origin.y - size.length,
                                         box.origin.z - size.height);
    struct size new_size = size_make(box.size.width + (2 * size.width),
                                     box.size.length + (2 * size.length),
                                     box.size.height + (2 * size.height));
    return box_make(new_origin, new_size);
}


struct box
box_extend_to_include_point(struct box box, struct point point)
{
    if (!box.size.width || !box.size.length || !box.size.height) {
        box.origin = point;
        box.size = size_make_unit();
    } else {        
        extend_to_include(&box.origin.x, &box.size.width, point.x);
        extend_to_include(&box.origin.y, &box.size.length, point.y);
        extend_to_include(&box.origin.z, &box.size.height, point.z);
    }
    return box;
}


struct box
box_for_area(struct point origin,
             enum direction direction,
             int length,
             int width,
             int left_offset,
             int padding)
{
    assert(left_offset < width);
    struct box box = box_make_empty(point_make(0, 0, 0));
    switch (direction) {
        case direction_north:
            box.origin = point_make(origin.x - left_offset - padding,
                                    origin.y,
                                    origin.z);
            box.size = size_make(width + (2 * padding), length + padding, 1);
            break;
        case direction_south:
            box.origin = point_make(origin.x + left_offset + padding,
                                    origin.y,
                                    origin.z);
            box.size = size_make(-width - (2 * padding), -length - padding, 1);
            break;
        case direction_east:
            box.origin = point_make(origin.x,
                                    origin.y + left_offset + padding,
                                    origin.z);
            box.size = size_make(length + padding, -width - (2 * padding), 1);
            break;
        case direction_west:
            box.origin = point_make(origin.x,
                                    origin.y - left_offset - padding,
                                    origin.z);
            box.size = size_make(-length - padding, width + (2 * padding), 1);
            break;
        default:
            fail("Unrecognized direction %i", direction);
            break;
    }
    return box_normalize(box);
}


int
box_index_for_point(struct box box, struct point point)
{
    assert(box_contains_point(box, point));
    
    return ((point.z - box.origin.z) * box.size.width * box.size.length)
         + ((point.y - box.origin.y) * box.size.width)
         + (point.x - box.origin.x);
}


struct box
box_intersection(struct box box, struct box other)
{
    box = box_normalize(box);
    other = box_normalize(other);
    
    struct point origin = point_make(max_int(box.origin.x, other.origin.x),
                                     max_int(box.origin.y, other.origin.y),
                                     max_int(box.origin.z, other.origin.z));
    
    struct point box_end = box_end_point(box);
    struct point other_end = box_end_point(other);
    struct point end = point_make(min_int(box_end.x, other_end.x),
                                  min_int(box_end.y, other_end.y),
                                  min_int(box_end.z, other_end.z));
    
    struct box intersection = box_make_from_points(origin, end);
    if (size_has_volume(intersection.size)) {
        return box;
    } else {
        return box_make_empty(origin);
    }
}


bool
box_intersects(struct box box, struct box other)
{
    struct box intersection = box_intersection(box, other);
    return size_has_volume(intersection.size);
}


struct box
box_make(struct point origin, struct size size)
{
    return (struct box){ .origin=origin, .size=size };
}


struct box
box_make_empty(struct point origin)
{
    return box_make(origin, size_make_empty());
}


struct box
box_make_from_boxes(struct box box, struct box other)
{
    struct point origin = point_min(box.origin, other.origin);
    struct point end = point_max(box_end_point(box), box_end_point(other));
    return box_make_from_points(origin, end);
    
}


struct box
box_make_from_points(struct point origin, struct point end)
{
    return box_make(origin, size_make_from_points(origin, end));
}


struct box
box_make_unit(struct point origin)
{
    return box_make(origin, size_make_unit());
}


struct box
box_normalize(struct box box)
{
    normalize(&box.origin.x, &box.size.width);
    normalize(&box.origin.y, &box.size.length);
    normalize(&box.origin.z, &box.size.height);
    return box;
}


int
box_volume(struct box box)
{
    assert(  (float)abs(box.size.width)
           * (float)abs(box.size.length)
           * (float)abs(box.size.height)
           <= (float)INT_MAX);
    return abs(box.size.width) * abs(box.size.length) * abs(box.size.height);
}


static void
extend_to_include(int *box_point, int *box_size, int point)
{
    if (point < *box_point) {
        int distance = *box_point - point;
        *box_point = point;
        *box_size += distance;
    } else {
        int distance = point - *box_point + 1;
        if (distance > *box_size) *box_size = distance;
    }
}
