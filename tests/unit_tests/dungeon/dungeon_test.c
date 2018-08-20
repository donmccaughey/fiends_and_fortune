#include <assert.h>

#include <base/base.h>

#include "dungeon/dungeon.h"
#include "dungeon/tile.h"


void
dungeon_test(void);


static void dungeon_generate_small_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    
    dungeon_generate_small(dungeon);
    
    struct box box = dungeon_box_for_level(dungeon, 1);
    assert(box_equals(box, box_make(point_make(-7, 0, 1), size_make(16, 15, 1))));
    
    struct tile *tile = dungeon_tile_at(dungeon, point_make(0, 0, 1));
    assert(tile);
    assert(tile_type_stairs_up == tile->type);
    
    tile = dungeon_tile_at(dungeon, point_make(0, 1, 1));
    assert(tile);
    assert(tile_type_stairs_up == tile->type);
    
    tile = dungeon_tile_at(dungeon, point_make(0, 2, 1));
    assert(tile);
    assert(tile_type_empty == tile->type);
    
    tile = dungeon_tile_at(dungeon, point_make(-1, -8, 1));
    assert(tile);
    assert(tile_type_filled == tile->type);
    
    dungeon_free(dungeon);
}


void dungeon_test(void)
{
    dungeon_generate_small_test();
}
