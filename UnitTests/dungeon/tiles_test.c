#include <assert.h>

#include "tile.h"
#include "tiles.h"


void
tiles_test(void);


static void tiles_add_tile_test(void)
{
    struct tiles *tiles = tiles_alloc();
    assert(0 == tiles_count(tiles));
    assert(range_is_equal(range_make(0, 0), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 0), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 0), tiles_z_range(tiles)));
    
    struct tile *tile1 = tile_alloc(point_make(0, 0, 0), tile_type_empty);
    tiles_add_tile(tiles, tile1);
    
    assert(1 == tiles_count(tiles));
    assert(tiles_tile_at_index(tiles, 0)->point.x == tile1->point.x);
    assert(range_is_equal(range_make(0, 1), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_z_range(tiles)));
    
    struct tile *tile2 = tile_alloc(point_make(1, 0, 0), tile_type_empty);
    tiles_add_tile(tiles, tile2);
    
    assert(2 == tiles_count(tiles));
    assert(tiles_tile_at_index(tiles, 1)->point.x == tile2->point.x);
    assert(range_is_equal(range_make(0, 2), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_z_range(tiles)));
    
    struct tile *tile3 = tile_alloc(point_make(-1, 0, 0), tile_type_empty);
    tiles_add_tile(tiles, tile3);
    
    assert(3 == tiles_count(tiles));
    assert(tiles_tile_at_index(tiles, 0)->point.x == tile3->point.x);
    assert(tiles_tile_at_index(tiles, 1)->point.x == tile1->point.x);
    assert(tiles_tile_at_index(tiles, 2)->point.x == tile2->point.x);
    assert(range_is_equal(range_make(-1, 2), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_z_range(tiles)));
    
    bool found = tiles_remove_tile(tiles, tile1);
    assert(found);
    assert(2 == tiles_count(tiles));
    assert(tiles_tile_at_index(tiles, 0)->point.x == tile3->point.x);
    assert(tiles_tile_at_index(tiles, 1)->point.x == tile2->point.x);
    assert(range_is_equal(range_make(-1, 2), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_z_range(tiles)));
    tile_free(tile1);
    
    found = tiles_remove_tile(tiles, tile2);
    assert(found);
    assert(1 == tiles_count(tiles));
    assert(tiles_tile_at_index(tiles, 0)->point.x == tile3->point.x);
    assert(range_is_equal(range_make(-1, 0), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 1), tiles_z_range(tiles)));
    tile_free(tile2);
    
    found = tiles_remove_tile(tiles, tile3);
    assert(found);
    assert(0 == tiles_count(tiles));
    assert(range_is_equal(range_make(0, 0), tiles_x_range(tiles)));
    assert(range_is_equal(range_make(0, 0), tiles_y_range(tiles)));
    assert(range_is_equal(range_make(0, 0), tiles_z_range(tiles)));
    tile_free(tile3);
    
    tiles_free(tiles);
}


static void tiles_alloc_with_parent_and_level_when_empty_test(void)
{
    struct tiles *tiles = tiles_alloc();
    struct tiles *level1 = tiles_alloc_with_parent_and_level(tiles, 1);
    
    assert(0 == tiles_count(level1));
    
    tiles_free(level1);
    tiles_free(tiles);
}


static void tiles_alloc_with_parent_and_level_one_tile_test(void)
{
    struct tiles *tiles = tiles_alloc();
    struct tile *tile1 = tile_alloc(point_make(0, 0, 1), tile_type_empty);
    tiles_add_tile(tiles, tile1);
    struct tiles *level1 = tiles_alloc_with_parent_and_level(tiles, 1);
    
    assert(tile_is_equal(tiles_tile_at_index(tiles, 0), tiles_tile_at_index(level1, 0)));
    assert(1 == tiles_count(level1));
    
    tiles_free(level1);
    tiles_free(tiles);
}


static void tiles_alloc_with_parent_and_level_one_tile_not_found_test(void)
{
    struct tiles *tiles = tiles_alloc();
    struct tile *tile1 = tile_alloc(point_make(0, 0, 1), tile_type_empty);
    tiles_add_tile(tiles, tile1);
    struct tiles *level1 = tiles_alloc_with_parent_and_level(tiles, 2);
    
    assert(0 == tiles_count(level1));
    
    tiles_free(level1);
    tiles_free(tiles);
}


static void tiles_alloc_with_parent_and_level_with_two_levels_test(void)
{
    struct tiles *tiles = tiles_alloc();
    struct tile *tile = tile_alloc(point_make(0, 0, 1), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(0, 1, 1), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(0, 0, 2), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(1, 0, 2), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(2, 0, 2), tile_type_empty);
    tiles_add_tile(tiles, tile);
    
    
    struct tiles *level0 = tiles_alloc_with_parent_and_level(tiles, 0);
    assert(0 == tiles_count(level0));
    tiles_free(level0);
    
    struct tiles *level1 = tiles_alloc_with_parent_and_level(tiles, 1);
    assert(tile_is_equal(tiles_tile_at_index(tiles, 0), tiles_tile_at_index(level1, 0)));
    assert(2 == tiles_count(level1));
    for (size_t i = 0; i < tiles_count(level1); ++i) {
        struct tile *level1Tile = tiles_tile_at_index(level1, i);
        assert(1 == level1Tile->point.z);
    }
    tiles_free(level1);
    
    struct tiles *level2 = tiles_alloc_with_parent_and_level(tiles, 2);
    assert(!tile_is_equal(tiles_tile_at_index(tiles, 0), tiles_tile_at_index(level2, 0)));
    assert(3 == tiles_count(level2));
    for (size_t i = 0; i < tiles_count(level2); ++i) {
        struct tile *level2Tile = tiles_tile_at_index(level2, i);
        assert(2 == level2Tile->point.z);
    }
    tiles_free(level2);
    
    struct tiles *level3 = tiles_alloc_with_parent_and_level(tiles, 3);
    assert(0 == tiles_count(level3));
    tiles_free(level3);
    
    tiles_free(tiles);
}


static void tiles_alloc_with_parent_and_level_with_two_noncontiguous_levels_test(void)
{
    struct tiles *tiles = tiles_alloc();
    struct tile *tile = tile_alloc(point_make(0, 0, 1), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(0, 1, 1), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(0, 0, 3), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(1, 0, 3), tile_type_empty);
    tiles_add_tile(tiles, tile);
    tile = tile_alloc(point_make(2, 0, 3), tile_type_empty);
    tiles_add_tile(tiles, tile);
    
    
    struct tiles *level0 = tiles_alloc_with_parent_and_level(tiles, 0);
    assert(0 == tiles_count(level0));
    tiles_free(level0);
    
    struct tiles *level1 = tiles_alloc_with_parent_and_level(tiles, 1);
    assert(tile_is_equal(tiles_tile_at_index(tiles, 0), tiles_tile_at_index(level1, 0)));
    assert(2 == tiles_count(level1));
    for (size_t i = 0; i < tiles_count(level1); ++i) {
        struct tile *level1Tile = tiles_tile_at_index(level1, i);
        assert(1 == level1Tile->point.z);
    }
    tiles_free(level1);
    
    struct tiles *level2 = tiles_alloc_with_parent_and_level(tiles, 2);
    assert(0 == tiles_count(level2));
    tiles_free(level2);
    
    struct tiles *level3 = tiles_alloc_with_parent_and_level(tiles, 3);
    assert(!tile_is_equal(tiles_tile_at_index(tiles, 0), tiles_tile_at_index(level3, 0)));
    assert(3 == tiles_count(level3));
    for (size_t i = 0; i < tiles_count(level3); ++i) {
        struct tile *level2Tile = tiles_tile_at_index(level3, i);
        assert(3 == level2Tile->point.z);
    }
    tiles_free(level3);
    
    struct tiles *level4 = tiles_alloc_with_parent_and_level(tiles, 4);
    assert(0 == tiles_count(level4));
    tiles_free(level4);
    
    tiles_free(tiles);
}


void
tiles_test(void)
{
    tiles_add_tile_test();
    tiles_alloc_with_parent_and_level_when_empty_test();
    tiles_alloc_with_parent_and_level_one_tile_test();
    tiles_alloc_with_parent_and_level_one_tile_not_found_test();
    tiles_alloc_with_parent_and_level_with_two_levels_test();
    tiles_alloc_with_parent_and_level_with_two_noncontiguous_levels_test();
}
