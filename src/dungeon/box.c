#include "box.h"


static inline int
max(int i, int j) {
    return i > j ? i : j;
}


static inline int
min(int i, int j) {
    return i < j ? i : j;
}


static inline struct point
point_max(struct point point, struct point other)
{
    return point_make(max(point.x, other.x),
                      max(point.y, other.y),
                      max(point.z, other.z));
}


static inline struct point
point_min(struct point point, struct point other)
{
    return point_make(min(point.x, other.x),
                      min(point.y, other.y),
                      min(point.z, other.z));
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
    struct size new_size = size_make(box.size.width + size.width,
                                     box.size.length + size.length,
                                     box.size.height + size.height);
    return box_make(new_origin, new_size);
}


struct box
box_make(struct point origin, struct size size)
{
    return (struct box){ .origin=origin, .size=size };
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
box_normalize(struct box box)
{
    if (box.size.width < 0) {
        box.origin.x -= box.size.width + 1;
        box.size.width = -box.size.width;
    }
    if (box.size.length < 0) {
        box.origin.y -= box.size.length + 1;
        box.size.length = -box.size.length;
    }
    if (box.size.height < 0) {
        box.origin.z -= box.size.height + 1;
        box.size.height = -box.size.height;
    }
    return box;
}
