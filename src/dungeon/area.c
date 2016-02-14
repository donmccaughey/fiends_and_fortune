#include "area.h"

#include "common/alloc_or_die.h"
#include "common/int.h"
#include "common/str.h"

#include "tile.h"


struct area *
area_alloc(enum area_type area_type,
           enum orientation orientation,
           struct box box,
           enum tile_type tile_type)
{
    struct area *area = calloc_or_die(1, sizeof(struct area));
    area->box = box;
    area->orientation = orientation;
    area->type = area_type;
    return area;
}


char *
area_alloc_description(struct area const *area)
{
    int width = area->box.size.width * 10;
    int length = area->box.size.length * 10;
    switch (area->type) {
        case area_type_chamber:
            return str_alloc_formatted("%u' x %u' chamber", width, length);
        case area_type_intersection:
            return strdup_or_die("intersection");
        case area_type_passage:
            if (orientation_east_to_west == area->orientation) {
                swap(&width, &length);
            }
            return str_alloc_formatted("%u' passage %s", length,
                                       orientation_name(area->orientation));
            break;
        case area_type_room:
            return str_alloc_formatted("%u' x %u' room", width, length);
        default:
            return str_alloc_formatted("%u' x %u' area", width, length);
    }
}


void
area_free(struct area *area)
{
    free_or_die(area);
}
