#include <assert.h>
#include <stdlib.h>
#include <base/base.h>
#include <dungeon/dungeon.h>
#include "tile.h"


void
tile_test(void);


static void
tile_alloc_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_filled);

    assert(direction_north == tile->direction);
    assert(tile_features_none == tile->features);
    assert(point_equals(point, tile->point));
    assert(tile_type_filled == tile->type);
    assert(wall_type_none == tile->walls.south);
    assert(wall_type_none == tile->walls.west);

    tile_free(tile);
}


static void
tile_alloc_copy_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);
    tile->features = tile_features_chimney_up;
    tile->walls.south = wall_type_door;

    struct tile *copy = tile_alloc_copy(tile);

    assert(direction_north == copy->direction);
    assert(tile_features_chimney_up == copy->features);
    assert(point_equals(point, copy->point));
    assert(tile_type_empty == copy->type);
    assert(wall_type_door == copy->walls.south);
    assert(wall_type_none == copy->walls.west);

    tile_free(copy);
    tile_free(tile);
}


static void
tile_equals_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile1 = tile_alloc(point, tile_type_empty);
    struct tile *tile2 = tile_alloc(point, tile_type_empty);

    assert(tile_equals(tile1, tile2));
    assert(tile_equals(tile2, tile1));

    tile2->direction = direction_east;

    assert( ! tile_equals(tile1, tile2));
    assert( ! tile_equals(tile2, tile1));

    tile2->direction = direction_north;
    tile2->features = tile_features_chimney_up;

    assert( ! tile_equals(tile1, tile2));
    assert( ! tile_equals(tile2, tile1));

    tile2->features = tile_features_none;
    tile2->point = point_make(4, 5, 6);

    assert( ! tile_equals(tile1, tile2));
    assert( ! tile_equals(tile2, tile1));

    tile2->point = point;
    tile2->type = tile_type_stairs_down;

    assert( ! tile_equals(tile1, tile2));
    assert( ! tile_equals(tile2, tile1));

    tile2->type = tile_type_empty;
    tile2->walls.south = wall_type_door;

    assert( ! tile_equals(tile1, tile2));
    assert( ! tile_equals(tile2, tile1));

    tile2->walls.south = wall_type_none;
    tile2->walls.west = wall_type_solid;

    assert( ! tile_equals(tile1, tile2));
    assert( ! tile_equals(tile2, tile1));

    tile_free(tile1);
    tile_free(tile2);
}


static void
tile_is_blank_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_filled);

    assert(tile_is_blank(tile));

    tile->type = tile_type_empty;

    assert( ! tile_is_blank(tile));

    tile->type = tile_type_filled;
    tile->walls.south = wall_type_solid;

    assert( ! tile_is_blank(tile));

    tile->walls.south = wall_type_none;
    tile->walls.west = wall_type_solid;

    assert( ! tile_is_blank(tile));

    tile_free(tile);
}


static void
tile_is_escavated_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);

    assert(tile_is_escavated(tile));

    tile->type = tile_type_stairs_down;

    assert(tile_is_escavated(tile));

    tile->type = tile_type_stairs_up;

    assert(tile_is_escavated(tile));

    tile->type = tile_type_filled;

    assert( ! tile_is_escavated(tile));

    tile_free(tile);
}


static void
tile_is_unescavated_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);

    assert( ! tile_is_unescavated(tile));

    tile->type = tile_type_stairs_down;

    assert( ! tile_is_unescavated(tile));

    tile->type = tile_type_stairs_up;

    assert( ! tile_is_unescavated(tile));

    tile->type = tile_type_filled;

    assert(tile_is_unescavated(tile));

    tile_free(tile);
}


static void
tile_has_south_exit_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);
    tile->walls.south = wall_type_solid;

    assert( ! tile_has_south_exit(tile));

    tile->walls.south = wall_type_secret_door;

    assert(tile_has_south_exit(tile));

    tile->walls.south = wall_type_door;

    assert(tile_has_south_exit(tile));

    tile->walls.south = wall_type_none;

    assert(tile_has_south_exit(tile));

    tile_free(tile);
}


static void
tile_has_south_wall_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);
    tile->walls.south = wall_type_solid;

    assert(tile_has_south_wall(tile));

    tile->walls.south = wall_type_secret_door;

    assert(tile_has_south_wall(tile));

    tile->walls.south = wall_type_door;

    assert(tile_has_south_wall(tile));

    tile->walls.south = wall_type_none;

    assert( ! tile_has_south_wall(tile));

    tile_free(tile);
}


static void
tile_has_west_exit_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);
    tile->walls.west = wall_type_solid;

    assert( ! tile_has_west_exit(tile));

    tile->walls.west = wall_type_secret_door;

    assert(tile_has_west_exit(tile));

    tile->walls.west = wall_type_door;

    assert(tile_has_west_exit(tile));

    tile->walls.west = wall_type_none;

    assert(tile_has_west_exit(tile));

    tile_free(tile);
}


static void
tile_has_west_wall_test(void)
{
    struct point point = point_make(1, 2, 3);
    struct tile *tile = tile_alloc(point, tile_type_empty);
    tile->walls.west = wall_type_solid;

    assert(tile_has_west_wall(tile));

    tile->walls.west = wall_type_secret_door;

    assert(tile_has_west_wall(tile));

    tile->walls.west = wall_type_door;

    assert(tile_has_west_wall(tile));

    tile->walls.west = wall_type_none;

    assert( ! tile_has_west_wall(tile));

    tile_free(tile);
}


static void
tile_add_to_array_sorted_by_point_test(void)
{
    struct tile **tiles = NULL;
    int count = 0;

    struct tile *tile1 = tile_alloc(point_make(1, 1, 1), tile_type_empty);
    tiles = tile_add_to_array_sorted_by_point(tiles, &count, tile1);

    assert(1 == count);
    assert(tiles);
    assert(tile1 == tiles[0]);

    struct tile *tile0 = tile_alloc(point_make(0, 1, 1), tile_type_empty);
    tiles = tile_add_to_array_sorted_by_point(tiles, &count, tile0);

    assert(2 == count);
    assert(tiles);
    assert(tile0 == tiles[0]);
    assert(tile1 == tiles[1]);

    struct tile *tile2 = tile_alloc(point_make(2, 1, 1), tile_type_empty);
    tiles = tile_add_to_array_sorted_by_point(tiles, &count, tile2);

    assert(3 == count);
    assert(tiles);
    assert(tile0 == tiles[0]);
    assert(tile1 == tiles[1]);
    assert(tile2 == tiles[2]);

    free(tiles);
    tile_free(tile0);
    tile_free(tile1);
    tile_free(tile2);
}


static void
tile_find_in_array_sorted_by_point_test(void)
{
    struct tile **tiles = NULL;
    int count = 0;
    struct tile **found = NULL;

    found = tile_find_in_array_sorted_by_point(tiles, count, point_make(1, 1, 1));
    assert( ! found);

    struct tile *tile0 = tile_alloc(point_make(0, 1, 1), tile_type_empty);
    struct tile *tile1 = tile_alloc(point_make(1, 1, 1), tile_type_empty);
    struct tile *tile2 = tile_alloc(point_make(2, 1, 1), tile_type_empty);
    tiles = tile_add_to_array_sorted_by_point(tiles, &count, tile0);
    tiles = tile_add_to_array_sorted_by_point(tiles, &count, tile1);
    tiles = tile_add_to_array_sorted_by_point(tiles, &count, tile2);

    found = tile_find_in_array_sorted_by_point(tiles, count, point_make(1, 1, 1));
    assert(found);
    assert(tile1 == *found);

    found = tile_find_in_array_sorted_by_point(tiles, count, point_make(3, 1, 1));
    assert( ! found);

    free(tiles);
    tile_free(tile0);
    tile_free(tile1);
    tile_free(tile2);
}


static void
tile_sort_array_by_point_test(void)
{
    struct tile *tile0 = tile_alloc(point_make(0, 1, 1), tile_type_empty);
    struct tile *tile1 = tile_alloc(point_make(1, 1, 1), tile_type_empty);
    struct tile *tile2 = tile_alloc(point_make(2, 1, 1), tile_type_empty);
    struct tile *tiles[] = { tile2, tile0, tile1 };
    int count = (int)(sizeof tiles / sizeof tiles[0]);

    tile_sort_array_by_point(tiles, count);

    assert(tile0 == tiles[0]);
    assert(tile1 == tiles[1]);
    assert(tile2 == tiles[2]);

    tile_free(tile0);
    tile_free(tile1);
    tile_free(tile2);
}


void
tile_test(void)
{
    tile_alloc_test();
    tile_alloc_copy_test();
    tile_equals_test();
    tile_is_blank_test();
    tile_is_escavated_test();
    tile_is_unescavated_test();
    tile_has_south_exit_test();
    tile_has_south_wall_test();
    tile_has_west_exit_test();
    tile_has_west_wall_test();
    tile_add_to_array_sorted_by_point_test();
    tile_find_in_array_sorted_by_point_test();
    tile_sort_array_by_point_test();
}
