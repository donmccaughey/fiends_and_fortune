#include "tiles_thumbnail.h"

#include <assert.h>
#include <base/base.h>

#include "tile.h"
#include "xmalloc.h"


void
tiles_thumbnail_test(void);


static void
tiles_thumbnail_directions_alloc_test_for_each_direction(void)
{
    int tiles_count = 8;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(-4, 0, 1), tile_type_empty);
    tiles[0]->direction = direction_north;
    tiles[1] = tile_alloc(point_make(-3, 0, 1), tile_type_empty);
    tiles[1]->direction = direction_south;
    tiles[2] = tile_alloc(point_make(-2, 0, 1), tile_type_empty);
    tiles[2]->direction = direction_east;
    tiles[3] = tile_alloc(point_make(-1, 0, 1), tile_type_empty);
    tiles[3]->direction = direction_west;
    tiles[4] = tile_alloc(point_make( 0, 0, 1), tile_type_empty);
    tiles[4]->direction = direction_northwest;
    tiles[5] = tile_alloc(point_make( 1, 0, 1), tile_type_empty);
    tiles[5]->direction = direction_northeast;
    tiles[6] = tile_alloc(point_make( 2, 0, 1), tile_type_empty);
    tiles[6]->direction = direction_southwest;
    tiles[7] = tile_alloc(point_make( 3, 0, 1), tile_type_empty);
    tiles[7]->direction = direction_southeast;

    char *thumbnail = tiles_thumbnail_directions_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "   -4 -3 -2 -1  0  1  2  3 \n"
            " 0  n  s  e  w nw ne sw se \n";
    assert(str_eq(expected, thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


static void
tiles_thumbnail_features_alloc_test_for_various_feature_combinations(void)
{
    int tiles_count = 4;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(0, 1, 1), tile_type_empty);
    tiles[1] = tile_alloc(point_make(0, 2, 1), tile_type_filled);
    tiles[2] = tile_alloc(point_make(0, 3, 1), tile_type_stairs_down);
    tiles[3] = tile_alloc(point_make(0,  4, 1), tile_type_stairs_up);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "    0 \n"
            " 4  ^ \n"
            " 3  v \n"
            " 2  : \n"
            " 1  . \n";
    assert(str_eq(expected, thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_each_tile_type(void)
{
    int tiles_count = 7;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(-2, 12, 1), tile_type_empty);
    tiles[0]->features = tile_features_none;
    tiles[1] = tile_alloc(point_make(-1, 12, 1), tile_type_filled);
    tiles[1]->features = tile_features_chimney_up;
    tiles[2] = tile_alloc(point_make( 0, 12, 1), tile_type_stairs_down);
    tiles[2]->features = tile_features_chimney_down;
    tiles[3] = tile_alloc(point_make( 1,  12, 1), tile_type_stairs_up);
    tiles[3]->features = tile_features_chute_entrance;
    tiles[4] = tile_alloc(point_make( 2,  12, 1), tile_type_stairs_up);
    tiles[4]->features = tile_features_chute_exit;
    tiles[5] = tile_alloc(point_make( 3,  12, 1), tile_type_stairs_up);
    tiles[5]->features = tile_features_chimney_up | tile_features_chimney_down;
    tiles[6] = tile_alloc(point_make( 4,  12, 1), tile_type_stairs_up);
    tiles[6]->features = tile_features_chute_entrance | tile_features_chute_exit;

    char *thumbnail = tiles_thumbnail_features_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "   -2 -1  0  1  2  3  4 \n"
            "12  0  1  2  4  8  3  c \n";
    assert(str_eq(expected, thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


static void
tiles_thumbnail_walls_alloc_test_for_each_wall_type(void)
{
    int tiles_count = 4;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(-1, -1, 4), tile_type_empty);
    tiles[0]->walls.south = wall_type_solid;
    tiles[0]->walls.west = wall_type_solid;
    tiles[1] = tile_alloc(point_make( 0, -1, 4), tile_type_empty);
    tiles[1]->walls.south = wall_type_secret_door;
    tiles[1]->walls.west = wall_type_secret_door;
    tiles[2] = tile_alloc(point_make( 1, -1, 4), tile_type_empty);
    tiles[2]->walls.south = wall_type_none;
    tiles[2]->walls.west = wall_type_none;
    tiles[3] = tile_alloc(point_make( 2, -1, 4), tile_type_empty);
    tiles[3]->walls.south = wall_type_door;
    tiles[3]->walls.west = wall_type_door;

    char *thumbnail = tiles_thumbnail_walls_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "   -1  0  1  2 \n"
            "-1 |_ $~ .. ]= \n";
    assert(str_eq(expected, thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_no_tiles(void)
{
    int tiles_count = 0;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    assert(str_eq("", thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_one_tile(void)
{
    int tiles_count = 1;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(7, 12, 1), tile_type_empty);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "    7 \n"
            "12  . \n";
    assert(str_eq(expected, thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


static void
tiles_thumbnail_types_alloc_test_for_multiple_tiles(void)
{
    int tiles_count = 5;
    struct tile **tiles = xcalloc(tiles_count, sizeof(struct tile *));
    tiles[0] = tile_alloc(point_make(0, 0, 4), tile_type_empty);
    tiles[1] = tile_alloc(point_make(5, 5, 4), tile_type_filled);
    tiles[2] = tile_alloc(point_make(2, 2, 4), tile_type_stairs_down);
    tiles[3] = tile_alloc(point_make(3, 2, 4), tile_type_stairs_down);
    tiles[4] = tile_alloc(point_make(4, 3, 4), tile_type_filled);

    char *thumbnail = tiles_thumbnail_types_alloc(tiles, tiles_count);
    assert(thumbnail);
    char const *expected =
            "    0  1  2  3  4  5 \n"
            " 5                 : \n"
            " 4                   \n"
            " 3              :    \n"
            " 2        v  v       \n"
            " 1                   \n"
            " 0  .                \n";
    assert(str_eq(expected, thumbnail));

    free(thumbnail);
    for (int i = 0; i < tiles_count; ++i) tile_free(tiles[i]);
    free(tiles);
}


void
tiles_thumbnail_test(void)
{
    tiles_thumbnail_directions_alloc_test_for_each_direction();
    tiles_thumbnail_features_alloc_test_for_various_feature_combinations();
    tiles_thumbnail_types_alloc_test_for_each_tile_type();
    tiles_thumbnail_walls_alloc_test_for_each_wall_type();

    tiles_thumbnail_types_alloc_test_for_no_tiles();
    tiles_thumbnail_types_alloc_test_for_one_tile();
    tiles_thumbnail_types_alloc_test_for_multiple_tiles();
}
