#include <assert.h>

#include "common/rnd.h"

#include "dungeon/dungeon.h"
#include "dungeon/tile.h"
#include "dungeon/tiles.h"


void
dungeon_test(void);


static void dungeon_generate_small_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    
    dungeon_generate_small(dungeon);
    
    assert(114 == dungeon->xtiles->count);
    
    assert(range_equals(range_make(-7, 9), dungeon->xtiles->x_range));
    assert(range_equals(range_make(0, 15), dungeon->xtiles->y_range));
    assert(114 == dungeon->xtiles->count);
    
    struct tile *tile = tiles_find_tile_at(dungeon->xtiles, point_make(0, 0, 1));
    assert(tile);
    
    tile = tiles_find_tile_at(dungeon->xtiles, point_make(0, 1, 1));
    assert(tile);
    
    tile = tiles_find_tile_at(dungeon->xtiles, point_make(-1, -8, 1));
    assert(!tile);
    
    dungeon_free(dungeon);
}


void dungeon_test(void)
{
    dungeon_generate_small_test();
}
