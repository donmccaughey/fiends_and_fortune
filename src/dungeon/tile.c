#include "tile.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"


struct tile *
tile_alloc(struct point point, enum tile_type type)
{
    struct tile *tile = calloc_or_die(1, sizeof(struct tile));
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
tile_has_south_wall(struct tile *tile)
{
    return wall_type_solid == tile->walls.south
        || wall_type_door == tile->walls.south;
}


bool
tile_has_west_wall(struct tile *tile)
{
    return wall_type_solid == tile->walls.west
        || wall_type_door == tile->walls.west;
}


bool
tile_is_blank(struct tile *tile)
{
    return tile_type_solid == tile->type
        && wall_type_none == tile->walls.south
        && wall_type_none == tile->walls.west;
}


bool
tile_is_unescavated(struct tile *tile)
{
    return tile_type_solid == tile->type;
}
