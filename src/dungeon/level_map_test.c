#include <assert.h>
#include <base/base.h>
#include <dungeon/dungeon.h>
#include <dungeon/text_rectangle.h>


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


void
level_map_test(void)
{
    level_map_alloc_test();
    level_map_tile_at_test();
    level_map_alloc_text_rectangle_test();
    level_map_alloc_text_rectangle_test_without_scale();
}
