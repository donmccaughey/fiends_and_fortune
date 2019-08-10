#include <assert.h>

#include <base/base.h>
#include <dungeon/dungeon.h>


void
dungeon_test(void);


static void
dungeon_alloc_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();

    assert(dungeon->areas);
    assert(0 == dungeon->areas_count);
    assert(dungeon->tiles);
    assert(0 == dungeon->tiles_count);

    dungeon_free(dungeon);
}

static void
dungeon_generate_small_test(void)
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


static void
dungeon_level_count_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile;

    assert(0 == dungeon_level_count(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(0, 0, 3));
    tile->type = tile_type_empty;
    assert(1 == dungeon_level_count(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(10, 10, 5));
    tile->type = tile_type_empty;
    assert(3 == dungeon_level_count(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(-5, -5, 4));
    tile->type = tile_type_empty;
    assert(3 == dungeon_level_count(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(5, 5, 2));
    tile->type = tile_type_empty;
    assert(4 == dungeon_level_count(dungeon));

    // TODO: test behavior with other tile types

    dungeon_free(dungeon);
}


static void
dungeon_starting_level_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile;

    assert(0 == dungeon_starting_level(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(0, 0, 3));
    tile->type = tile_type_empty;
    assert(3 == dungeon_starting_level(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(-5, -5, 4));
    tile->type = tile_type_empty;
    assert(3 == dungeon_starting_level(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(5, 5, 2));
    tile->type = tile_type_empty;
    assert(2 == dungeon_starting_level(dungeon));

    // TODO: test behavior with other tile types

    dungeon_free(dungeon);
}


static void
dungeon_ending_level_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile;

    assert(0 == dungeon_ending_level(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(0, 0, 3));
    tile->type = tile_type_empty;
    assert(3 == dungeon_ending_level(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(-5, -5, 4));
    tile->type = tile_type_empty;
    assert(4 == dungeon_ending_level(dungeon));

    tile = dungeon_tile_at(dungeon, point_make(5, 5, 7));
    tile->type = tile_type_empty;
    assert(7 == dungeon_ending_level(dungeon));

    // TODO: test behavior with other tile types

    dungeon_free(dungeon);
}


static void
dungeon_box_for_level_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile;
    struct box box, expected;

    expected = box_make(point_make(0, 0, 2), size_make(0, 0, 1));
    box = dungeon_box_for_level(dungeon, 2);
    assert(box_equals(expected, box));

    tile = dungeon_tile_at(dungeon, point_make(5, 5, 2));
    expected = box_make(point_make(0, 0, 2), size_make(0, 0, 1));
    box = dungeon_box_for_level(dungeon, 2);
    assert(box_equals(expected, box));

    tile->type = tile_type_empty;
    expected = box_make(point_make(5, 5, 2), size_make(1, 1, 1));
    box = dungeon_box_for_level(dungeon, 2);
    assert(box_equals(expected, box));

    tile = dungeon_tile_at(dungeon, point_make(10, 5, 2));
    tile->type = tile_type_empty;
    expected = box_make(point_make(5, 5, 2), size_make(6, 1, 1));
    box = dungeon_box_for_level(dungeon, 2);
    assert(box_equals(expected, box));

    tile = dungeon_tile_at(dungeon, point_make(10, 2, 2));
    tile->type = tile_type_empty;
    expected = box_make(point_make(5, 2, 2), size_make(6, 4, 1));
    box = dungeon_box_for_level(dungeon, 2);
    assert(box_equals(expected, box));

    dungeon_free(dungeon);
}


void dungeon_test(void)
{
    dungeon_alloc_test();
    dungeon_generate_small_test();
    dungeon_level_count_test();
    dungeon_starting_level_test();
    dungeon_ending_level_test();
    dungeon_box_for_level_test();
}
