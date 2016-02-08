#include <assert.h>

#include "tile.h"
#include "tiles.h"


void
tiles_test(void);


static void tiles_add_tile_test(void)
{
    struct tiles *tiles = tiles_alloc();
    assert(0 == tiles->count);
    
    struct tile *tile1 = tile_alloc(point_make(0, 0, 0), tile_type_empty);
    tiles_add_tile(tiles, tile1);
    assert(1 == tiles->count);
    assert(tiles->members[0]->point.x == tile1->point.x);
    
    struct tile *tile2 = tile_alloc(point_make(1, 0, 0), tile_type_empty);
    tiles_add_tile(tiles, tile2);
    assert(2 == tiles->count);
    assert(tiles->members[1]->point.x == tile2->point.x);
    
    struct tile *tile3 = tile_alloc(point_make(-1, 0, 0), tile_type_empty);
    tiles_add_tile(tiles, tile3);
    assert(3 == tiles->count);
    assert(tiles->members[0]->point.x == tile3->point.x);
    assert(tiles->members[1]->point.x == tile1->point.x);
    assert(tiles->members[2]->point.x == tile2->point.x);
    
    bool found = tiles_remove_tile(tiles, tile1);
    assert(found);
    assert(2 == tiles->count);
    assert(tiles->members[0]->point.x == tile3->point.x);
    assert(tiles->members[1]->point.x == tile2->point.x);
    tile_free(tile1);
    
    found = tiles_remove_tile(tiles, tile2);
    assert(found);
    assert(1 == tiles->count);
    assert(tiles->members[0]->point.x == tile3->point.x);
    tile_free(tile2);
    
    found = tiles_remove_tile(tiles, tile3);
    assert(found);
    assert(0 == tiles->count);
    tile_free(tile3);
    
    tiles_free(tiles);
}


void
tiles_test(void)
{
    tiles_add_tile_test();
}
