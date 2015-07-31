#include "tile.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"


struct tile *
tile_alloc(struct point point, enum tile_type type)
{
    struct tile *tile = malloc_or_die(sizeof(struct tile));
    tile->point = point;
    tile->type = type;
    return tile;
}


void
tile_free(struct tile *tile)
{
    free_or_die(tile);
}


bool
tile_is_equal(struct tile *tile, struct tile *other_tile)
{
    return tile->point.x == other_tile->point.x
        && tile->point.y == other_tile->point.y
        && tile->point.z == other_tile->point.z
        && tile->type == other_tile->type;
}
