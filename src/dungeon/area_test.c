#include "area.h"

#include <assert.h>


void
area_test(void);


static void
area_alloc_test(void)
{
    struct box box = box_make(point_make(1, 2, 3), size_make(4, 5, 1));
    struct area *area = area_alloc(area_type_room, direction_north, box, tile_type_empty);

    assert(1 == area->box.origin.x);
    assert(2 == area->box.origin.y);
    assert(3 == area->box.origin.z);

    assert(4 == area->box.size.width);
    assert(5 == area->box.size.length);
    assert(1 == area->box.size.height);

    assert(direction_north == area->direction);
    assert(area_features_none == area->features);
    assert(area_type_room == area->type);

    area_free(area);
}


void
area_test(void)
{
    area_alloc_test();
}
