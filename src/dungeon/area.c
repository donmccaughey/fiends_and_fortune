#include "area.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/str.h"

#include "dungeon.h"
#include "tile.h"
#include "tiles.h"


static char *
alloc_description(enum area_type area_type,
                  enum orientation orientation,
                  struct range x_range,
                  struct range y_range)
{
    int length;
    int width;
    if (orientation_east_to_west == orientation) {
        length = range_length(x_range);
        width = range_length(y_range);
    } else {
        length = range_length(y_range);
        width = range_length(x_range);
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


void
area_init(struct area *area,
          struct dungeon *dungeon,
          enum area_type area_type,
          enum orientation orientation,
          enum tile_type tile_type,
          struct range x_range,
          struct range y_range,
          int level)
{
    area->dungeon = dungeon;
    area->description = alloc_description(area_type,
                                          orientation,
                                          x_range,
                                          y_range);
    area->orientation = orientation;
    area->type = area_type;
    
    area->tiles_count = range_length(x_range) * range_length(y_range);
    area->tiles = calloc_or_die(area->tiles_count, sizeof(struct tile *));
    
    int i = 0;
    for (int y = y_range.begin; y < y_range.end; ++y) {
        for (int x = x_range.begin; x < x_range.end; ++x) {
            area->tiles[i] = dungeon_add_tile(dungeon,
                                              point_make(x, y, level),
                                              tile_type);
            ++i;
        }
    }
}


void
area_fin(struct area *area)
{
    free_or_die(area->description);
    free_or_die(area->tiles);
}
