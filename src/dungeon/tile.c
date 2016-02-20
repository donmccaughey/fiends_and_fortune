#include "tile.h"

#include <stdlib.h>

#include "common/alloc_or_die.h"


static int
compare_point_to_tile(void const *key, void const *object)
{
    struct point const *point = key;
    struct tile *const *tile = object;
    return point_compare(*point, (*tile)->point);
}


static int
compare_by_point(void const *object, void const *other_object)
{
    struct tile *const *tile = object;
    struct tile *const *other = other_object;
    return point_compare((*tile)->point, (*other)->point);
}


struct tile **
tile_add_to_array_sorted_by_point(struct tile **tiles,
                                  int *count,
                                  struct tile *tile)
{
    int index = *count;
    ++*count;
    tiles = reallocarray_or_die(tiles, *count, sizeof(struct tile *));
    tiles[index] = tile;
    tile_sort_array_by_point(tiles, *count);
    return tiles;
}


struct tile *
tile_alloc(struct point point, enum tile_type type)
{
    struct tile *tile = calloc_or_die(1, sizeof(struct tile));
    tile->point = point;
    tile->type = type;
    return tile;
}


struct tile *
tile_alloc_copy(struct tile *tile)
{
    struct tile *copy = calloc_or_die(1, sizeof(struct tile));
    *copy = *tile;
    return copy;
}


bool
tile_equals(struct tile const *tile, struct tile const *other)
{
    return point_equals(tile->point, other->point)
        && tile->type == other->type;
}


struct tile **
tile_find_in_array_sorted_by_point(struct tile **tiles,
                                   int count,
                                   struct point point)
{
    return bsearch(&point, tiles, count, sizeof(struct tile *),
                   compare_point_to_tile);
}


void
tile_free(struct tile *tile)
{
    free_or_die(tile);
}


bool
tile_has_south_exit(struct tile const *tile)
{
    return wall_type_none == tile->walls.south
        || wall_type_door == tile->walls.south
        || wall_type_secret_door == tile->walls.south;
}


bool
tile_has_south_wall(struct tile const *tile)
{
    return wall_type_solid == tile->walls.south
        || wall_type_door == tile->walls.south
        || wall_type_secret_door == tile->walls.south;
}


bool
tile_has_west_exit(struct tile const *tile)
{
    return wall_type_none == tile->walls.west
        || wall_type_door == tile->walls.west
        || wall_type_secret_door == tile->walls.west;
}


bool
tile_has_west_wall(struct tile const *tile)
{
    return wall_type_solid == tile->walls.west
        || wall_type_door == tile->walls.west
        || wall_type_secret_door == tile->walls.west;
}


bool
tile_is_blank(struct tile const *tile)
{
    return tile_type_filled == tile->type
        && wall_type_none == tile->walls.south
        && wall_type_none == tile->walls.west;
}


bool
tile_is_escavated(struct tile const *tile)
{
    return tile_type_filled != tile->type;
}


bool
tile_is_unescavated(struct tile const *tile)
{
    return tile_type_filled == tile->type;
}


void
tile_sort_array_by_point(struct tile **tiles, int count)
{
    qsort(tiles, count, sizeof(struct tile *), compare_by_point);
}
