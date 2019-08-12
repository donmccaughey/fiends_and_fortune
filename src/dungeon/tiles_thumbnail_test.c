#include <assert.h>
#include <base/base.h>
#include "tile.h"
#include "tiles_thumbnail.h"


void
tiles_thumbnail_test(void);


static void
tiles_thumbnail_types_alloc_test_for_no_tiles(void)
{
    int tiles_count = 0;
    struct tile **tiles = calloc_or_die(tiles_count, sizeof(struct tile *));

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    assert(str_eq("", thumbnail));

    free_or_die(thumbnail);
    free_or_die(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_one_tiles(void)
{
    int tiles_count = 1;
    struct tile **tiles = calloc_or_die(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(0, 0, 1), tile_type_empty);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "    0 \n"
            " 0  . \n"
            ;
    assert(str_eq(expected, thumbnail));

    free_or_die(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free_or_die(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_two_tiles(void)
{
    int tiles_count = 2;
    struct tile **tiles = calloc_or_die(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(0, 0, 1), tile_type_empty);
    tiles[1] = tile_alloc(point_make(1, 0, 1), tile_type_filled);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "    0  1 \n"
            " 0  .  : \n";
    assert(str_eq(expected, thumbnail));

    free_or_die(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free_or_die(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_three_tiles(void)
{
    int tiles_count = 3;
    struct tile **tiles = calloc_or_die(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(0, 0, 1), tile_type_empty);
    tiles[1] = tile_alloc(point_make(1, 0, 1), tile_type_filled);
    tiles[2] = tile_alloc(point_make(0, -1, 1), tile_type_stairs_up);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "    0  1 \n"
            " 0  .  : \n"
            "-1  ^    \n";
    assert(str_eq(expected, thumbnail));

    free_or_die(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free_or_die(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_each_tile_type(void)
{
    int tiles_count = 5;
    struct tile **tiles = calloc_or_die(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(-1, -1, 1), tile_type_empty);
    tiles[1] = tile_alloc(point_make( 0, -1, 1), tile_type_stairs_up);
    tiles[2] = tile_alloc(point_make( 1, -1, 1), tile_type_stairs_down);
    tiles[3] = tile_alloc(point_make( 0,  0, 1), tile_type_filled);
    tiles[4] = tile_alloc(point_make(-1,  1, 1), tile_type_filled);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "   -1  0  1 \n"
            " 1  :       \n"
            " 0     :    \n"
            "-1  .  ^  v \n";
    assert(str_eq(expected, thumbnail));

    free_or_die(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free_or_die(tiles);
}


void
tiles_thumbnail_test(void)
{
    tiles_thumbnail_types_alloc_test_for_no_tiles();
    tiles_thumbnail_types_alloc_test_for_one_tiles();
    tiles_thumbnail_types_alloc_test_for_two_tiles();
    tiles_thumbnail_types_alloc_test_for_three_tiles();
    tiles_thumbnail_types_alloc_test_for_each_tile_type();
}
