#include "tiles_thumbnail.h"

#include <base/base.h>
#include "box.h"
#include "text_rectangle.h"
#include "tile.h"


typedef void print_tile_fn(struct text_rectangle *, struct tile *);


static void
print_tile_direction(struct text_rectangle *text_rectangle, struct tile *tile)
{
    char const *direction;
    switch (tile->direction) {
        case direction_north:     direction = " n"; break;
        case direction_northeast: direction = "ne"; break;
        case direction_east:      direction = " e"; break;
        case direction_southeast: direction = "se"; break;
        case direction_south:     direction = " s"; break;
        case direction_southwest: direction = "sw"; break;
        case direction_west:      direction = " w"; break;
        case direction_northwest: direction = "nw"; break;
        default:                  direction = ""; break;
    }
    text_rectangle_print_format(text_rectangle, "%-2s ", direction);
}


static void
print_tile_features(struct text_rectangle *text_rectangle, struct tile *tile)
{
    text_rectangle_print_format(text_rectangle, " %x ", tile->features);
}


static void
print_tile_type(struct text_rectangle *text_rectangle, struct tile *tile)
{
    char type;
    switch (tile->type) {
        case tile_type_filled:      type = ':'; break;
        case tile_type_empty:       type = '.'; break;
        case tile_type_stairs_down: type = 'v'; break;
        case tile_type_stairs_up:   type = '^'; break;
        default:                    type = ' '; break;
    }
    text_rectangle_print_format(text_rectangle, " %c ", type);
}


static void
print_tile_walls(struct text_rectangle *text_rectangle, struct tile *tile)
{
    char south_wall;
    switch (tile->walls.south) {
        case wall_type_none:        south_wall = '.'; break;
        case wall_type_solid:       south_wall = '_'; break;
        case wall_type_door:        south_wall = '='; break;
        case wall_type_secret_door: south_wall = '~'; break;
        default:                    south_wall = ' '; break;
    }
    char west_wall;
    switch (tile->walls.west) {
        case wall_type_none:        west_wall = '.'; break;
        case wall_type_solid:       west_wall = '|'; break;
        case wall_type_door:        west_wall = ']'; break;
        case wall_type_secret_door: west_wall = '$'; break;
        default:                    west_wall = ' '; break;
    }
    text_rectangle_print_format(text_rectangle, "%c%c ", west_wall, south_wall);
}


static char *
tiles_thumbnail_alloc(struct tile **tiles, int tiles_count, print_tile_fn print_tile)
{
    if (!tiles_count) return str_alloc_empty();

    struct box box = box_make_unit(tiles[0]->point);
    for (int i = 0; i < tiles_count; ++i) {
        box = box_extend_to_include_point(box, tiles[i]->point);
    }

    int column_count = 3 * (1 + box.size.width);
    int row_count = 1 + box.size.length;
    struct text_rectangle *text_rectangle = text_rectangle_alloc(column_count, row_count);

    for (int i = 0; i < box.size.width; ++i) {
        int column_index = 3 * (1 + i);
        int row_index = 0;
        text_rectangle_move_to(text_rectangle, column_index, row_index);
        int x = box.origin.x + i;
        text_rectangle_print_format(text_rectangle, "%2i ", x);
    }

    for (int j = 0; j < box.size.length; ++j) {
        int column_index = 0;
        int row_index = 1 + j;
        text_rectangle_move_to(text_rectangle, column_index, row_index);
        int y = box.origin.y + box.size.length - 1 - j;
        text_rectangle_print_format(text_rectangle, "%2i ", y);
    }

    for (int i = 0; i < tiles_count; ++i) {
        struct tile *tile = tiles[i];
        int column_index = 3 * (1 + tile->point.x - box.origin.x);
        int row_index = 1 + (box.size.length - 1) - (tile->point.y - box.origin.y);
        text_rectangle_move_to(text_rectangle, column_index, row_index);
        print_tile(text_rectangle, tile);
    }

    char *thumbnail = xstrdup(text_rectangle->chars);
    text_rectangle_free(text_rectangle);
    return thumbnail;
}


char *
tiles_thumbnail_directions_alloc(struct tile **tiles, int tiles_count)
{
    return tiles_thumbnail_alloc(tiles, tiles_count, print_tile_direction);
}


char *
tiles_thumbnail_features_alloc(struct tile **tiles, int tiles_count)
{
    return tiles_thumbnail_alloc(tiles, tiles_count, print_tile_features);
}


char *
tiles_thumbnail_types_alloc(struct tile **tiles, int tiles_count)
{
    return tiles_thumbnail_alloc(tiles, tiles_count, print_tile_type);
}


char *
tiles_thumbnail_walls_alloc(struct tile **tiles, int tiles_count)
{
    return tiles_thumbnail_alloc(tiles, tiles_count, print_tile_walls);
}
