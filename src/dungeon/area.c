#include "area.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "tile.h"
#include "tiles.h"


void
area_add_tiles(struct area *area,
               enum tile_type type,
               struct range x_range,
               struct range y_range,
               int32_t z)
{
    assert(!tiles_has_tile_in_range(tiles_root(area->tiles), x_range, y_range, range_make(z, z + 1)));
    for (int32_t y = y_range.begin; y < y_range.end; ++y) {
        for (int32_t x = x_range.begin; x < x_range.end; ++x) {
            assert(!tiles_find_tile_at(tiles_root(area->tiles), point_make(x, y, z)));
            
            struct tile *tile = tile_alloc(point_make(x, y, z), type);
            tiles_add_tile(area->tiles, tile);
        }
    }
}


struct area *
area_alloc(char const *description,
           struct tiles *parent_tiles,
           enum area_type type)
{
    struct area *area = malloc_or_die(sizeof(struct area));
    area->description = strdup_or_die(description);
    area->tiles = tiles_alloc_with_parent(parent_tiles);
    area->type = type;
    return area;
}


void
area_free(struct area *area)
{
    free_or_die(area->description);
    tiles_free(area->tiles);
    free_or_die(area);
}
