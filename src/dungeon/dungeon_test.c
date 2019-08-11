#include <assert.h>

#include <base/base.h>
#include <dungeon/dungeon.h>
#include "tile.h"


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


static void
dungeon_add_area_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct box box;
    struct area *area;

    assert(0 == dungeon->areas_count);

    box = box_make(point_make(0, 0, 1), size_make(3, 4, 1));
    area = area_alloc(area_type_chamber, direction_north, box);
    dungeon_add_area(dungeon, area);

    assert(1 == dungeon->areas_count);
    assert(area == dungeon->areas[0]);

    box = box_make(point_make(10, 10, 1), size_make(1, 6, 1));
    area = area_alloc(area_type_passage, direction_north, box);
    dungeon_add_area(dungeon, area);

    assert(2 == dungeon->areas_count);
    assert(area == dungeon->areas[1]);
    assert(area_type_chamber == dungeon->areas[0]->type);

    dungeon_free(dungeon);
}


static void
dungeon_is_box_excavated_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct box box;
    struct tile *tile;

    box = box_make(point_make(0, 0, 1), size_make(3, 4, 1));
    assert(!dungeon_is_box_excavated(dungeon, box));

    tile = dungeon_tile_at(dungeon, point_make(3, 0, 1));
    tile->type = tile_type_empty;
    assert(!dungeon_is_box_excavated(dungeon, box));

    tile = dungeon_tile_at(dungeon, point_make(2, 0, 1));
    tile->type = tile_type_empty;
    assert(dungeon_is_box_excavated(dungeon, box));

    tile->type = tile_type_filled;
    assert(!dungeon_is_box_excavated(dungeon, box));

    tile = dungeon_tile_at(dungeon, point_make(1, 1, 1));
    tile->type = tile_type_stairs_down;
    assert(dungeon_is_box_excavated(dungeon, box));

    tile->type = tile_type_stairs_up;
    assert(dungeon_is_box_excavated(dungeon, box));

    tile->type = tile_type_filled;
    assert(!dungeon_is_box_excavated(dungeon, box));

    dungeon_free(dungeon);
}


static void
dungeon_alloc_tiles_for_box_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct box box;
    struct tile **tiles;

    box = box_make(point_make(0, 0, 1), size_make(2, 3, 1));
    tiles = dungeon_alloc_tiles_for_box(dungeon, box);

    assert(tiles);
    assert(point_equals(point_make(0, 0, 1), tiles[0]->point));
    assert(point_equals(point_make(1, 0, 1), tiles[1]->point));
    assert(point_equals(point_make(0, 1, 1), tiles[2]->point));
    assert(point_equals(point_make(1, 1, 1), tiles[3]->point));
    assert(point_equals(point_make(0, 2, 1), tiles[4]->point));
    assert(point_equals(point_make(1, 2, 1), tiles[5]->point));
    free_or_die(tiles);

    box = box_make(point_make(0, 0, 1), size_make(2, 1, 3));
    tiles = dungeon_alloc_tiles_for_box(dungeon, box);

    assert(tiles);
    assert(point_equals(point_make(0, 0, 1), tiles[0]->point));
    assert(point_equals(point_make(1, 0, 1), tiles[1]->point));
    assert(point_equals(point_make(0, 0, 2), tiles[2]->point));
    assert(point_equals(point_make(1, 0, 2), tiles[3]->point));
    assert(point_equals(point_make(0, 0, 3), tiles[4]->point));
    assert(point_equals(point_make(1, 0, 3), tiles[5]->point));
    free_or_die(tiles);

    dungeon_free(dungeon);
}


static void
dungeon_tile_at_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile;

    tile = dungeon_tile_at(dungeon, point_make(0, 0, 1));
    assert(tile);
    assert(tile_type_filled == tile->type);
    assert(tile_features_none == tile->features);
    assert(direction_north == tile->direction);
    assert(wall_type_none == tile->walls.south);
    assert(wall_type_none == tile->walls.west);
    assert(point_equals(point_make(0, 0, 1), tile->point));
    tile->type = tile_type_empty;
    tile->walls.south = wall_type_solid;
    tile->walls.west = wall_type_solid;

    tile = dungeon_tile_at(dungeon, point_make(0, 0, 1));
    assert(tile);
    assert(tile_type_empty == tile->type);
    assert(tile_features_none == tile->features);
    assert(direction_north == tile->direction);
    assert(wall_type_solid == tile->walls.south);
    assert(wall_type_solid == tile->walls.west);
    assert(point_equals(point_make(0, 0, 1), tile->point));

    dungeon_free(dungeon);
}


static void
dungeon_alloc_text_rectangle_for_level_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile = dungeon_tile_at(dungeon, point_make(0, 0, 1));
    tile->type = tile_type_empty;

    struct text_rectangle *text_rectangle = dungeon_alloc_text_rectangle_for_level(dungeon, 1);
    assert(text_rectangle);
    assert(9 == text_rectangle->row_count);
    assert(21 == text_rectangle->column_count);
    char const *expected =
        "     -1   0   1      \n"
        "    +---+---+---+    \n"
        "  1 |:::::::::::| 1  \n"
        "    +:::::::::::+    \n"
        "  0 |:::    ::::| 0  \n"
        "    +:::.   ::::+    \n"
        " -1 |:::::::::::| -1 \n"
        "    +---+---+---+    \n"
        "     -1   0   1      \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
    dungeon_free(dungeon);
}


static void
dungeon_alloc_descriptions_of_entrances_and_exits_for_level_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct box box;
    struct area *area;
    struct ptr_array *descriptions;

    descriptions = dungeon_alloc_descriptions_of_entrances_and_exits_for_level(dungeon, 1);
    assert(descriptions);
    assert(0 == descriptions->count);
    ptr_array_free(descriptions);

    box = box_make(point_make(4, 4, 1), size_make(2, 2, 1));
    area = area_alloc(area_type_chamber, direction_north, box);
    dungeon_add_area(dungeon, area);

    descriptions = dungeon_alloc_descriptions_of_entrances_and_exits_for_level(dungeon, 1);
    assert(descriptions);
    assert(0 == descriptions->count);
    ptr_array_free(descriptions);

    box = box_make(point_make(0, 0, 1), size_make(1, 2, 1));
    area = area_alloc(area_type_stairs_down, direction_north, box);
    dungeon_add_area(dungeon, area);

    descriptions = dungeon_alloc_descriptions_of_entrances_and_exits_for_level(dungeon, 1);
    assert(descriptions);
    assert(1 == descriptions->count);
    assert(str_eq("(0, 0)       stairs down to level 2", descriptions->elements[0]));
    ptr_array_clear(descriptions, free_or_die);
    ptr_array_free(descriptions);

    box = box_make(point_make(10, 10, 1), size_make(3, 3, 1));
    area = area_alloc(area_type_room, direction_north, box);
    area->features = area_features_chimney_up;
    dungeon_add_area(dungeon, area);

    descriptions = dungeon_alloc_descriptions_of_entrances_and_exits_for_level(dungeon, 1);
    assert(descriptions);
    assert(2 == descriptions->count);
    assert(str_eq("(0, 0)       stairs down to level 2", descriptions->elements[0]));
    assert(str_eq("(11, 11)     30' x 30' room, chimney up to surface", descriptions->elements[1]));
    ptr_array_clear(descriptions, free_or_die);
    ptr_array_free(descriptions);

    dungeon_free(dungeon);
}


static void
dungeon_alloc_descriptions_of_chambers_and_rooms_for_level_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct box box;
    struct area *area;
    struct ptr_array *descriptions;

    descriptions = dungeon_alloc_descriptions_of_chambers_and_rooms_for_level(dungeon, 1);
    assert(descriptions);
    assert(0 == descriptions->count);
    ptr_array_free(descriptions);

    box = box_make(point_make(0, 0, 1), size_make(1, 2, 1));
    area = area_alloc(area_type_stairs_down, direction_north, box);
    dungeon_add_area(dungeon, area);

    descriptions = dungeon_alloc_descriptions_of_chambers_and_rooms_for_level(dungeon, 1);
    assert(descriptions);
    assert(0 == descriptions->count);
    ptr_array_free(descriptions);

    box = box_make(point_make(4, 4, 1), size_make(2, 2, 1));
    area = area_alloc(area_type_chamber, direction_north, box);
    dungeon_add_area(dungeon, area);

    descriptions = dungeon_alloc_descriptions_of_chambers_and_rooms_for_level(dungeon, 1);
    assert(descriptions);
    assert(1 == descriptions->count);
    assert(str_eq("(4, 4)       20' x 20' chamber", descriptions->elements[0]));
    ptr_array_clear(descriptions, free_or_die);
    ptr_array_free(descriptions);

    box = box_make(point_make(10, 10, 1), size_make(3, 3, 1));
    area = area_alloc(area_type_room, direction_north, box);
    area->features = area_features_chimney_up;
    dungeon_add_area(dungeon, area);

    descriptions = dungeon_alloc_descriptions_of_chambers_and_rooms_for_level(dungeon, 1);
    assert(descriptions);
    assert(2 == descriptions->count);
    assert(str_eq("(4, 4)       20' x 20' chamber", descriptions->elements[0]));
    assert(str_eq("(11, 11)     30' x 30' room, chimney up to surface", descriptions->elements[1]));
    ptr_array_clear(descriptions, free_or_die);
    ptr_array_free(descriptions);

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
    dungeon_add_area_test();
    dungeon_is_box_excavated_test();
    dungeon_alloc_tiles_for_box_test();
    dungeon_tile_at_test();
    dungeon_alloc_text_rectangle_for_level_test();
    dungeon_alloc_descriptions_of_entrances_and_exits_for_level_test();
    dungeon_alloc_descriptions_of_chambers_and_rooms_for_level_test();
}
