#include "area.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/str.h"

#include "tile.h"
#include "tiles.h"


static char *
alloc_description(struct area const *area)
{
    int length;
    int width;
    if (orientation_east_to_west == area->orientation) {
        length = range_length(area->tiles->x_range);
        width = range_length(area->tiles->y_range);
    } else {
        length = range_length(area->tiles->y_range);
        width = range_length(area->tiles->x_range);
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
        default:
            return str_alloc_formatted("%u' x %u' area",
                                       length * 10, width * 10);
    }
}


struct area *
area_alloc(struct tiles *parent_tiles,
           enum area_type area_type,
           enum orientation orientation,
           enum tile_type tile_type,
           struct range x_range,
           struct range y_range,
           int z)
{
    struct area *area = malloc_or_die(sizeof(struct area));
    area->tiles = tiles_alloc_with_parent(parent_tiles);
    area->type = area_type;
    area->orientation = orientation;
    for (int y = y_range.begin; y < y_range.end; ++y) {
        for (int x = x_range.begin; x < x_range.end; ++x) {
            struct tile *tile = tile_alloc(point_make(x, y, z), tile_type);
            tiles_add_tile(area->tiles, tile);
        }
    }
    area->description = alloc_description(area);
    return area;
}


void
area_free(struct area *area)
{
    free_or_die(area->description);
    tiles_free(area->tiles);
    free_or_die(area);
}
