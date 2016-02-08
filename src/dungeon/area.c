#include "area.h"

#include "common/alloc_or_die.h"
#include "common/str.h"

#include "dungeon.h"
#include "tile.h"


static char *
alloc_description(enum area_type area_type,
                  enum orientation orientation,
                  struct box box)
{
    int width;
    int length;
    if (orientation_east_to_west == orientation) {
        width = box.size.length;
        length = box.size.width;
    } else {
        width = box.size.width;
        length = box.size.length;
    }
    switch (area_type) {
        case area_type_chamber:
            return str_alloc_formatted("%u' x %u' chamber",
                                       length * 10, width * 10);
        case area_type_intersection:
            return strdup_or_die("intersection");
        case area_type_passage:
            return str_alloc_formatted("%u' passage %s",
                                       length * 10,
                                       orientation_name(orientation));
            break;
        default:
            return str_alloc_formatted("%u' x %u' area",
                                       length * 10, width * 10);
    }
}


struct area *
area_alloc(struct dungeon *dungeon,
           enum area_type area_type,
           enum orientation orientation,
           struct box box,
           enum tile_type tile_type)
{
    struct area *area = calloc_or_die(1, sizeof(struct area));
    area->box = box;
    area->description = alloc_description(area_type, orientation, box);
    area->orientation = orientation;
    area->type = area_type;
    
    area->tiles_count = box_volume(box);
    area->tiles = dungeon_alloc_tiles_for_box(dungeon, box);
    for (int i = 0; i < area->tiles_count; ++i) {
        area->tiles[i]->type = tile_type;
    }
    
    struct point end = box_end_point(area->box);
    
    for (int i = 0; i < area->box.size.width; ++i) {
        int x = area->box.origin.x + i;
        struct point point = point_make(x, area->box.origin.y, area->box.origin.z);
        int index = box_index_for_point(area->box, point);
        struct tile *tile = area->tiles[index];
        tile->walls.south = wall_type_solid;
        
        point = point_make(x, end.y, area->box.origin.z);
        tile = dungeon_tile_at(dungeon, point);
        tile->walls.south = wall_type_solid;
    }
    
    for (int j = 0; j < area->box.size.length; ++j) {
        int y = area->box.origin.y + j;
        struct point point = point_make(area->box.origin.x, y, area->box.origin.z);
        int index = box_index_for_point(area->box, point);
        struct tile *tile = area->tiles[index];
        tile->walls.west = wall_type_solid;
        
        point = point_make(end.x, y, area->box.origin.z);
        tile = dungeon_tile_at(dungeon, point);
        tile->walls.west = wall_type_solid;
    }
    
    return area;
}


void
area_free(struct area *area)
{
    if (area) {
        free_or_die(area->description);
        free_or_die(area->tiles);
        free_or_die(area);
    }
}
