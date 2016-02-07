#ifndef FNF_DUNGEON_BOX_H_INCLUDED
#define FNF_DUNGEON_BOX_H_INCLUDED


#include "point.h"
#include "size.h"


struct box {
    struct point origin;
    struct size size;
};


struct box
box_make(struct point origin, struct size size);

struct box
box_make_from_boxes(struct box box, struct box other);

struct box
box_make_from_points(struct point origin, struct point end);

struct box
box_normalize(struct box box);

struct point
box_end_point(struct box box);

struct box
box_expand(struct box box, struct size size);



#endif
