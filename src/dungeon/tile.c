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
tile_equals(struct tile *tile, struct tile *other)
{
    return tile->point.x == other->point.x
        && tile->point.y == other->point.y
        && tile->point.z == other->point.z
        && tile->type == other->type;
}


bool
tile_is_unescavated(struct tile *tile)
{
    return tile_type_solid == tile->type;
}
