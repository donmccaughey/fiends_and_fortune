#include <assert.h>
#include <dungeon/dungeon.h>

#include "astr.h"
#include "tile.h"


void
level_map_test(void);


static void
level_map_alloc_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct level_map *level_map = level_map_alloc(dungeon, 1);

    assert(dungeon == level_map->dungeon);
    assert(point_equals(point_make(-1, -1, 1), level_map->box.origin));
    assert(size_equals(size_make(2, 2, 1), level_map->box.size));
    assert(level_map->tiles);

    level_map_free(level_map);
    dungeon_free(dungeon);
}


static void
level_map_tile_at_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct level_map *level_map = level_map_alloc(dungeon, 1);
    struct tile *tile;

    tile = level_map_tile_at(level_map, point_make(4, 4, 1));
    assert(!tile);

    tile = level_map_tile_at(level_map, point_make(0, 0, 1));
    assert(tile);
    assert(point_equals(point_make(0, 0, 1), tile->point));

    level_map_free(level_map);
    dungeon_free(dungeon);
}


static void
level_map_alloc_text_rectangle_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct level_map *level_map = level_map_alloc(dungeon, 1);
    bool show_scale = true;

    struct text_rectangle *text_rectangle = level_map_alloc_text_rectangle(level_map, show_scale);

    assert(text_rectangle);
    char const *expected =
            "     -1   0      \n"
            "    +---+---+    \n"
            "  0 |:::::::| 0  \n"
            "    +:::::::+    \n"
            " -1 |:::::::| -1 \n"
            "    +---+---+    \n"
            "     -1   0      \n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
    dungeon_free(dungeon);
}


static void
level_map_alloc_text_rectangle_test_without_scale(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct level_map *level_map = level_map_alloc(dungeon, 1);
    bool show_scale = false;

    struct text_rectangle *text_rectangle = level_map_alloc_text_rectangle(level_map, show_scale);

    assert(text_rectangle);
    char const *expected =
            "+---+---+\n"
            "|:::::::|\n"
            "+:::::::+\n"
            "|:::::::|\n"
            "+---+---+\n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
    dungeon_free(dungeon);
}


static void
level_map_alloc_text_rectangle_test_with_tile_added(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct tile *tile = dungeon_tile_at(dungeon, point_make(2, 3, 1));
    tile->type = tile_type_empty;
    struct level_map *level_map = level_map_alloc(dungeon, 1);
    bool show_scale = true;

    struct text_rectangle *text_rectangle = level_map_alloc_text_rectangle(level_map, show_scale);

    assert(text_rectangle);
    char const *expected =
            "      1   2   3      \n"
            "    +---+---+---+    \n"
            "  4 |:::::::::::| 4  \n"
            "    +:::::::::::+    \n"
            "  3 |:::    ::::| 3  \n"
            "    +:::.   ::::+    \n"
            "  2 |:::::::::::| 2  \n"
            "    +---+---+---+    \n"
            "      1   2   3      \n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
    dungeon_free(dungeon);
}


static void
level_map_calculate_text_rectangle_dimensions_test(void)
{
    struct size level_map_size;
    bool show_scale;
    int column_count;
    int row_count;

    level_map_size = size_make(2, 2, 1);
    show_scale = true;
    level_map_calculate_text_rectangle_dimensions(level_map_size, show_scale, &column_count, &row_count);

    assert(17 == column_count);
    assert(7 == row_count);

    show_scale = false;
    level_map_calculate_text_rectangle_dimensions(level_map_size, show_scale, &column_count, &row_count);

    assert(9 == column_count);
    assert(5 == row_count);

    level_map_size = size_make(4, 5, 1);
    show_scale = true;
    level_map_calculate_text_rectangle_dimensions(level_map_size, show_scale, &column_count, &row_count);

    assert(25 == column_count);
    assert(13 == row_count);
}


static void
level_map_print_border_row_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(18, 2);
    text_rectangle_fill(text_rectangle, '.');
    struct size level_map_size;
    bool show_scale;
    char const *expected;

    level_map_size = size_make(2, 2, 1);
    show_scale = true;
    level_map_print_border_row(level_map_size, text_rectangle, show_scale);

    expected =
            "    +---+---+    .\n"
            "..................\n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_next_row(text_rectangle);
    level_map_size = size_make(4, 3, 1);
    show_scale = false;
    level_map_print_border_row(level_map_size, text_rectangle, show_scale);

    expected =
            "    +---+---+    .\n"
            "+---+---+---+---+.\n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
}


static void
level_map_print_scale_row_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(26, 2);
    text_rectangle_fill(text_rectangle, '.');
    struct box level_map_box;
    char const *expected;

    level_map_box = box_make(point_make(2, 2, 1),
                             size_make(2, 2, 1));
    level_map_print_scale_row(level_map_box, text_rectangle);

    expected =
            "      2   3      .........\n"
            "..........................\n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_next_row(text_rectangle);
    level_map_box = box_make(point_make(-2, 3, 1),
                             size_make(4, 3, 1));
    level_map_print_scale_row(level_map_box, text_rectangle);

    expected =
            "      2   3      .........\n"
            "     -2  -1   0   1      .\n";
    assert(astr_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
}


static void
level_map_fill_tile_bottom_half_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();

    struct tile *extent;
    extent = dungeon_tile_at(dungeon, point_make(1, 3, 1));
    extent->type = tile_type_empty;
    extent = dungeon_tile_at(dungeon, point_make(3, 1, 1));
    extent->type = tile_type_empty;
    // produces a level map with origin (0, 0, 1) and size (5, 5, 1)

    struct level_map *level_map = level_map_alloc(dungeon, 1);

    struct tile *tile = level_map_tile_at(level_map, point_make(1, 1, 1));

    char half_tile[5];

    // no walls
    tile->walls.south = wall_type_none;
    tile->walls.west = wall_type_none;

    tile->type = tile_type_empty;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(".   ", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_north;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  ^ ", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_south;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  v ", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_east;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(" > >", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_west;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(" < <", half_tile));

    tile->type = tile_type_stairs_up;
    tile->direction = direction_north;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("====", half_tile));

    tile->type = tile_type_stairs_up;
    tile->direction = direction_east;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("IIII", half_tile));

    tile->type = tile_type_filled;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("::::", half_tile));

    // south wall
    tile->type = tile_type_empty;
    tile->walls.west = wall_type_none;

    struct tile *west_tile = level_map_tile_at(level_map, point_make(0, 1, 1));
    west_tile->type = tile_type_empty;
    west_tile->walls.south = wall_type_solid;

    tile->walls.south = wall_type_solid;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("----", half_tile));

    tile->walls.south = wall_type_door;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("-[-]", half_tile));

    tile->walls.south = wall_type_secret_door;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("--s-", half_tile));

    west_tile->type = tile_type_filled;
    west_tile->walls.south = wall_type_none;

    // west wall
    tile->walls.south = wall_type_none;
    tile->walls.west = wall_type_solid;

    struct tile *south_tile = level_map_tile_at(level_map, point_make(1, 0, 1));
    south_tile->walls.west = wall_type_solid;

    tile->type = tile_type_filled;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("|:::", half_tile));

    tile->type = tile_type_empty;
    level_map_fill_tile_bottom_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("|   ", half_tile));

    south_tile->walls.west = wall_type_none;

    level_map_free(level_map);
    dungeon_free(dungeon);
}


// TODO: test level_map_fill_tile_bottom_half() for corner cases


static void
level_map_fill_tile_top_half_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();

    struct tile *extent;
    extent = dungeon_tile_at(dungeon, point_make(1, 3, 1));
    extent->type = tile_type_empty;
    extent = dungeon_tile_at(dungeon, point_make(3, 1, 1));
    extent->type = tile_type_empty;
    // produces a level map with origin (0, 0, 1) and size (5, 5, 1)

    struct level_map *level_map = level_map_alloc(dungeon, 1);

    struct tile *tile = level_map_tile_at(level_map, point_make(1, 1, 1));

    char half_tile[5];

    // no walls
    tile->walls.south = wall_type_none;
    tile->walls.west = wall_type_none;

    tile->type = tile_type_empty;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("    ", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_north;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  ^ ", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_south;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  v ", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_east;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(" > >", half_tile));

    tile->type = tile_type_stairs_down;
    tile->direction = direction_west;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(" < <", half_tile));

    tile->type = tile_type_stairs_up;
    tile->direction = direction_north;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("====", half_tile));

    tile->type = tile_type_stairs_up;
    tile->direction = direction_east;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("IIII", half_tile));

    tile->type = tile_type_filled;
    tile->direction = direction_east;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("::::", half_tile));

    // west wall
    tile->type = tile_type_empty;

    tile->walls.west = wall_type_solid;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("|   ", half_tile));

    tile->walls.west = wall_type_door;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("|]  ", half_tile));

    tile->walls.west = wall_type_secret_door;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("$   ", half_tile));

    // features
    tile->type = tile_type_empty;
    tile->walls.west = wall_type_none;

    tile->features = tile_features_chimney_down;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  o ", half_tile));

    tile->features = tile_features_chimney_up;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(" ( )", half_tile));

    tile->features = tile_features_chimney_up | tile_features_chimney_down;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq(" (o)", half_tile));

    tile->features = tile_features_chute_entrance;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  @ ", half_tile));

    tile->features = tile_features_chute_exit;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("  * ", half_tile));

    tile->features = tile_features_none;

    // east door
    struct tile *east_tile = level_map_tile_at(level_map, point_make(2, 1, 1));
    east_tile->walls.west = wall_type_door;
    level_map_fill_tile_top_half(level_map, point_make(1, 1, 1), half_tile);
    assert(astr_eq("   [", half_tile));

    level_map_free(level_map);
    dungeon_free(dungeon);
}


void
level_map_tile_has_sw_corner_test()
{
    struct dungeon *dungeon = dungeon_alloc();

    struct tile *extent;
    extent = dungeon_tile_at(dungeon, point_make(1, 3, 1));
    extent->type = tile_type_empty;
    extent = dungeon_tile_at(dungeon, point_make(3, 1, 1));
    extent->type = tile_type_empty;
    // produces a level map with origin (0, 0, 1) and size (5, 5, 1)

    struct level_map *level_map = level_map_alloc(dungeon, 1);

    struct tile *tile = level_map_tile_at(level_map, point_make(1, 1, 1));
    struct tile *south_tile = level_map_tile_at(level_map, point_make(1, 0, 1));
    struct tile *west_tile = level_map_tile_at(level_map, point_make(0, 1, 1));

    // has no walls
    tile->walls.west = wall_type_none;
    tile->walls.south = wall_type_none;

    // ::::....
    // ::::....
    // ::::::::
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_none;
    assert(!level_map_tile_has_sw_corner(level_map, tile));

    // ::::....
    // ----+...
    // ::::::::
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_solid;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    // ::::....
    // ::::+...
    // ::::|:::
    south_tile->walls.west = wall_type_solid;
    west_tile->walls.south = wall_type_none;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    // ::::....
    // ----+...
    // ::::|:::
    south_tile->walls.west = wall_type_solid;
    west_tile->walls.south = wall_type_solid;
    assert(level_map_tile_has_sw_corner(level_map, tile));


    // has only south wall
    tile->walls.west = wall_type_none;
    tile->walls.south = wall_type_solid;

    // ::::....
    // ::::+---
    // ::::::::
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_none;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    // ::::....
    // --------
    // ::::::::
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_solid;
    assert(!level_map_tile_has_sw_corner(level_map, tile));

    // ::::....
    // ::::+---
    // ::::|:::
    south_tile->walls.west = wall_type_solid;
    west_tile->walls.south = wall_type_none;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    // ::::....
    // ----+---
    // ::::|:::
    south_tile->walls.west = wall_type_solid;
    west_tile->walls.south = wall_type_solid;
    assert(level_map_tile_has_sw_corner(level_map, tile));


    // has only west wall
    tile->walls.west = wall_type_solid;
    tile->walls.south = wall_type_none;

    // ::::|...
    // ::::+...
    // ::::::::
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_none;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    // ::::|...
    // ----+...
    // ::::::::
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_solid;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    // ::::|...
    // ::::|...
    // ::::|:::
    south_tile->walls.west = wall_type_solid;
    west_tile->walls.south = wall_type_none;
    assert(!level_map_tile_has_sw_corner(level_map, tile));

    // ::::|...
    // ----+...
    // ::::|:::
    south_tile->walls.west = wall_type_solid;
    west_tile->walls.south = wall_type_solid;
    assert(level_map_tile_has_sw_corner(level_map, tile));


    // has both south and west walls
    // ::::|
    // ::::+---
    // ::::::::
    tile->walls.south = wall_type_solid;
    tile->walls.west = wall_type_solid;
    south_tile->walls.west = wall_type_none;
    west_tile->walls.south = wall_type_none;
    assert(level_map_tile_has_sw_corner(level_map, tile));

    level_map_free(level_map);
    dungeon_free(dungeon);
}


void
level_map_test(void)
{
    level_map_alloc_test();
    level_map_tile_at_test();
    level_map_alloc_text_rectangle_test();
    level_map_alloc_text_rectangle_test_without_scale();
    level_map_alloc_text_rectangle_test_with_tile_added();
    level_map_calculate_text_rectangle_dimensions_test();
    level_map_print_border_row_test();
    level_map_print_scale_row_test();
    level_map_fill_tile_bottom_half_test();
    level_map_fill_tile_top_half_test();
    level_map_tile_has_sw_corner_test();
}
