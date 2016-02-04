#include "tiles.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "common/alloc_or_die.h"

#include "tile.h"


static void
append_tile(struct tiles *tiles, struct tile *tile);

static int
tile_compare_by_point(void const *item1, void const *item2);

static struct tile **
find(struct tiles const *tiles, struct tile const *criteria);

static void
sort(struct tiles *tiles);

static void
update_ranges(struct tiles *tiles);


void
tiles_add_tile(struct tiles *tiles, struct tile *tile)
{
    if (tiles->parent) tiles_add_tile(tiles->parent, tile);
    append_tile(tiles, tile);
    sort(tiles);
}


static void
append_tile(struct tiles *tiles, struct tile *tile)
{
    assert(!tiles_find_tile_at(tiles, tile->point));
    
    int index = tiles->count;
    assert(index >= 0);
    ++tiles->count;
    tiles->members = reallocarray_or_die(tiles->members,
                                         tiles->count,
                                         sizeof(struct tile *));    
    tiles->members[index] = tile;
    tiles->x_range = range_extend_to_include_value(tiles->x_range, tile->point.x);
    tiles->y_range = range_extend_to_include_value(tiles->y_range, tile->point.y);
    tiles->z_range = range_extend_to_include_value(tiles->z_range, tile->point.z);
}


static int
tile_compare_by_point(void const *item1, void const *item2)
{
    struct tile *const *pointer1 = item1;
    struct tile *const *pointer2 = item2;
    struct tile *tile1 = *pointer1;
    struct tile *tile2 = *pointer2;
    
    return point_compare(tile1->point, tile2->point);
}


struct tiles *
tiles_alloc_with_parent(struct tiles *parent)
{
    struct tiles *tiles = tiles_alloc();
    tiles->parent = parent;
    return tiles;
}


struct tiles *
tiles_alloc(void)
{
    struct tiles *tiles = calloc_or_die(1, sizeof(struct tiles));
    tiles->members = calloc_or_die(1, sizeof(struct tile *));
    return tiles;
}


struct tiles *
tiles_alloc_tiles_for_level(struct tiles *parent, int32_t level)
{
    struct tiles *tiles = tiles_alloc_with_parent(parent);
    // TODO: replace linear search with binary lower/upper bound search
    // and memcpy the whole block of tiles
    for (size_t i = 0; i < parent->count; ++i) {
        struct tile *tile = parent->members[i];
        if (tile->point.z == level) {
            append_tile(tiles, tile);
        } else if (tile->point.z > level) {
            break;
        }
    }
    return tiles;
}


void
tiles_free(struct tiles *tiles)
{
    if (!tiles->parent) {
        for (int i = 0; i < tiles->count; ++i) {
            tile_free(tiles->members[i]);
        }
    }
    free_or_die(tiles->members);
    free_or_die(tiles);
}


static struct tile **
find(struct tiles const *tiles, struct tile const *criteria)
{
    return bsearch(&criteria, tiles->members, tiles->count, sizeof(struct tile *), tile_compare_by_point);
}


struct tile *
tiles_find_tile_at(struct tiles const *tiles, struct point point)
{
    struct tile tile = { .point = point };
    struct tile **tileInTiles = find(tiles, &tile);
    return tileInTiles ? *tileInTiles : NULL;
}


bool
tiles_has_tile_in_range(struct tiles *tiles,
                        struct range x_range,
                        struct range y_range,
                        struct range z_range)
{
    for (size_t i = 0; i < tiles->count; ++i) {
        struct tile *tile = tiles->members[i];
        if (   range_contains(x_range, tile->point.x)
            && range_contains(y_range, tile->point.y)
            && range_contains(z_range, tile->point.z))
        {
            return true;
        }
        if (tile->point.z >= z_range.end) break;
    }
    return false;
}


bool
tiles_remove_tile(struct tiles *tiles, struct tile const *tile)
{
    struct tile **found = find(tiles, tile);
    if (!found) return false;
    
    struct tile **next = found + 1;
    struct tile **end = tiles->members + tiles->count;
    if (next < end) {
        size_t size = (end - next) * sizeof(struct tile *);
        memmove(found, next, size);
    }
    --tiles->count;
    tiles->members = reallocarray_or_die(tiles->members, tiles->count, sizeof(struct tile *));
    update_ranges(tiles);
    
    return tiles->parent ? tiles_remove_tile(tiles->parent, tile) : true;
}


struct tiles *
tiles_root(struct tiles *tiles)
{
    while (tiles->parent) tiles = tiles->parent;
    return tiles;
}


static void
sort(struct tiles *tiles)
{
    qsort(tiles->members, tiles->count, sizeof(struct tile *), tile_compare_by_point);
}


static void
update_ranges(struct tiles *tiles)
{
    tiles->x_range = range_make(0, 0);
    tiles->y_range = range_make(0, 0);
    tiles->z_range = range_make(0, 0);
    for (size_t i = 0; i < tiles->count; ++i) {
        struct tile *tile = tiles->members[i];
        tiles->x_range = range_extend_to_include_value(tiles->x_range, tile->point.x);
        tiles->y_range = range_extend_to_include_value(tiles->y_range, tile->point.y);
        tiles->z_range = range_extend_to_include_value(tiles->z_range, tile->point.z);
    }
}
