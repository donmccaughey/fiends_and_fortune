#include <assert.h>
#include <base/base.h>
#include <dungeon/dungeon.h>
#include <dungeon/text_rectangle.h>
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
    assert(str_eq(expected, text_rectangle->chars));

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
    assert(str_eq(expected, text_rectangle->chars));

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
    assert(str_eq(expected, text_rectangle->chars));

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


void
level_map_test(void)
{
    level_map_alloc_test();
    level_map_tile_at_test();
    level_map_alloc_text_rectangle_test();
    level_map_alloc_text_rectangle_test_without_scale();
    level_map_alloc_text_rectangle_test_with_tile_added();
    level_map_calculate_text_rectangle_dimensions_test();
}
