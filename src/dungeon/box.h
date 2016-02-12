#ifndef FNF_DUNGEON_BOX_H_INCLUDED
#define FNF_DUNGEON_BOX_H_INCLUDED


#include <stdbool.h>

#include "point.h"
#include "size.h"


struct box {
    struct point origin;
    struct size size;
};


struct box
box_make(struct point origin, struct size size);

struct box
box_make_empty(struct point origin);

struct box
box_make_unit(struct point origin);

struct box
box_make_from_boxes(struct box box, struct box other);

struct box
box_make_from_points(struct point origin, struct point end);

bool
box_equals(struct box box, struct box other);

bool
box_contains_box(struct box box, struct box other);

bool
box_contains_point(struct box box, struct point point);

bool
box_intersects(struct box box, struct box other);

struct point
box_end_point(struct box box);

int
box_area(struct box box);

int
box_volume(struct box box);

struct box
box_normalize(struct box box);

struct box
box_expand(struct box box, struct size size);

struct box
box_extend_to_include_point(struct box box, struct point point);

struct box
box_intersection(struct box box, struct box other);

int
box_index_for_point(struct box box, struct point point);


#endif
