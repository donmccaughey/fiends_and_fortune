#include "tiles.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "tile.h"


static int
tile_compare_by_point(void const *item1, void const *item2);

static struct tile **
find(struct tiles const *tiles, struct tile const *criteria);


void
tiles_add_tile(struct tiles *tiles, struct tile *tile)
{
    assert(!tiles_find_tile_at(tiles, tile->point));
    
    int index = tiles->count;
    assert(index >= 0);
    ++tiles->count;
    tiles->members = reallocarray_or_die(tiles->members,
                                         tiles->count,
                                         sizeof(struct tile *));
    tiles->members[index] = tile;
    qsort(tiles->members,
          tiles->count,
          sizeof(struct tile *),
          tile_compare_by_point);
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
tiles_alloc(void)
{
    struct tiles *tiles = calloc_or_die(1, sizeof(struct tiles));
    tiles->members = calloc_or_die(1, sizeof(struct tile *));
    return tiles;
}


void
tiles_free(struct tiles *tiles)
{
    for (int i = 0; i < tiles->count; ++i) {
        tile_free(tiles->members[i]);
    }
    free_or_die(tiles->members);
    free_or_die(tiles);
}


static struct tile **
find(struct tiles const *tiles, struct tile const *criteria)
{
    return bsearch(&criteria,
                   tiles->members,
                   tiles->count,
                   sizeof(struct tile *), tile_compare_by_point);
}


struct tile *
tiles_find_tile_at(struct tiles const *tiles, struct point point)
{
    struct tile tile = { .point = point };
    struct tile **tileInTiles = find(tiles, &tile);
    return tileInTiles ? *tileInTiles : NULL;
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
    return true;
}
