#include "area.h"

#include "common/alloc_or_die.h"
#include "common/str.h"

#include "dungeon.h"
#include "tile.h"


struct area *
area_alloc(struct dungeon *dungeon,
           enum area_type area_type,
           enum orientation orientation,
           struct box box,
           enum tile_type tile_type)
{
    struct area *area = calloc_or_die(1, sizeof(struct area));
    area->box = box;
    area->orientation = orientation;
    area->type = area_type;
    
    area->tiles_count = box_volume(box);
    area->tiles = dungeon_alloc_tiles_for_box(dungeon, box);
    for (int i = 0; i < area->tiles_count; ++i) {
        area->tiles[i]->type = tile_type;
    }
    dungeon_set_walls(dungeon, box, wall_type_solid);
    
    return area;
}


char *
area_alloc_description(struct area const *area)
{
    int width;
    int length;
    if (orientation_east_to_west == area->orientation) {
        width = area->box.size.length;
        length = area->box.size.width;
    } else {
        width = area->box.size.width;
        length = area->box.size.length;
    }
    switch (area->type) {
        case area_type_chamber:
            return str_alloc_formatted("%u' x %u' chamber",
                                       length * 10, width * 10);
        case area_type_intersection:
            return strdup_or_die("intersection");
        case area_type_passage:
            return str_alloc_formatted("%u' passage %s",
                                       length * 10,
                                       orientation_name(area->orientation));
            break;
        case area_type_room:
            return str_alloc_formatted("%u' x %u' room",
                                       length * 10, width * 10);
        default:
            return str_alloc_formatted("%u' x %u' area",
                                       length * 10, width * 10);
    }
}


void
area_free(struct area *area)
{
    if (area) {
        free_or_die(area->tiles);
        free_or_die(area);
    }
}
