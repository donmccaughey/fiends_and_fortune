#include <assert.h>

#include "common/rnd.h"

#include "dungeon/dungeon.h"
#include "dungeon/tile.h"
#include "dungeon/tiles.h"


void
dungeon_test(void);


static void dungeon_generate_small_test(void)
{
    struct dungeon dungeon;
    dungeon_initialize(&dungeon);
    
    dungeon_generate_small(&dungeon);
    
    assert(113 == tiles_count(dungeon.tiles));
    
    assert(range_is_equal(range_make(-7, 9), tiles_x_range(dungeon.tiles)));
    assert(range_is_equal(range_make(0, 15), tiles_y_range(dungeon.tiles)));
    assert(113 == tiles_count(dungeon.tiles));
    
    struct tile *tile = tiles_find_tile_at(dungeon.tiles, point_make(0, 0, 1));
    assert(tile);
    
    tile = tiles_find_tile_at(dungeon.tiles, point_make(0, 1, 1));
    assert(tile);
    
    tile = tiles_find_tile_at(dungeon.tiles, point_make(-1, -8, 1));
    assert(!tile);
    
    dungeon_finalize(&dungeon);
}


void dungeon_test(void)
{
    dungeon_generate_small_test();
}
