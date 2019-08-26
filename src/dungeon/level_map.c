#include "level_map.h"

#include <assert.h>
#include <background/background.h>
#include <base/base.h>

#include "dungeon.h"
#include "text_rectangle.h"
#include "tile.h"
#include "tile_type.h"
#include "wall_type.h"


#define LMARGIN     "    "
#define LMARGIN_NUM "%3i "
#define RMARGIN     "    "
#define RMARGIN_NUM "| %-3i\n"


static void
tile_bottom_half(struct level_map const *level_map,
                 struct point point,
                 char half_tile[5]);

static void
tile_top_half(struct level_map const *level_map,
              struct point point,
              char half_tile[5]);


static void
fill_half_tile(struct tile *tile, char half_tile[5])
{
    switch (tile->type) {
        case tile_type_filled: strcpy(half_tile, "::::"); break;
        case tile_type_empty: strcpy(half_tile, "    "); break;
        case tile_type_stairs_down:
            switch (tile->direction) {
                case direction_north: strcpy(half_tile, "  ^ "); break;
                case direction_south: strcpy(half_tile, "  v "); break;
                case direction_east: strcpy(half_tile, " > >"); break;
                case direction_west: strcpy(half_tile, " < <"); break;
                default: break;
            }
            break;
        case tile_type_stairs_up:
            switch (orientation_from_direction(tile->direction)) {
                case orientation_north_to_south: strcpy(half_tile, "===="); break;
                case orientation_east_to_west: strcpy(half_tile, "IIII"); break;
                default: break;
            }
            break;
        default: strcpy(half_tile, "::::"); break;
    }
}


struct level_map *
level_map_alloc(struct dungeon *dungeon, int level)
{
    struct level_map *level_map = calloc_or_die(1, sizeof(struct level_map));
    level_map->dungeon = dungeon;
    
    struct box box = dungeon_box_for_level(dungeon, level);
    level_map->box = box_expand(box, size_make(1, 1, 0));
    level_map->tiles = dungeon_alloc_tiles_for_box(dungeon, level_map->box);
    
    return level_map;
}


struct text_rectangle *
level_map_alloc_text_rectangle(struct level_map *level_map, bool show_scale)
{
    int column_count;
    int row_count;
    level_map_calculate_text_rectangle_dimensions(level_map->box.size,
                                                  show_scale,
                                                  &column_count,
                                                  &row_count);
    struct text_rectangle *text_rectangle = text_rectangle_alloc(column_count,
                                                                 row_count);
    if (show_scale) {
        level_map_print_scale_row(level_map->box, text_rectangle);
        text_rectangle_next_row(text_rectangle);
    }

    level_map_print_border_row(level_map->box.size, text_rectangle, show_scale);
    
    // map tiles
    struct point point = level_map->box.origin;
    point.y += level_map->box.size.length;
    char half_tile[5];
    for (int j = 0; j < level_map->box.size.length; ++j) {
        --point.y;
        // top line of row
        text_rectangle_next_row(text_rectangle);
        if (show_scale) text_rectangle_print_format(text_rectangle, LMARGIN_NUM, point.y);
        point.x = level_map->box.origin.x;
        for (int i = 0; i < level_map->box.size.width; ++i) {
            tile_top_half(level_map, point, half_tile);
            text_rectangle_print_format(text_rectangle, half_tile);
            ++point.x;
        }
        if (show_scale) {
            text_rectangle_print_format(text_rectangle, RMARGIN_NUM, point.y);
        } else {
            text_rectangle_print_format(text_rectangle, "|");
        }
        
        // bottom line of row
        text_rectangle_next_row(text_rectangle);
        if (show_scale) text_rectangle_print_format(text_rectangle, LMARGIN);
        point.x = level_map->box.origin.x;
        for (int i = 0; i < level_map->box.size.width; ++i) {
            tile_bottom_half(level_map, point, half_tile);
            text_rectangle_print_format(text_rectangle, half_tile);
            ++point.x;
        }
        text_rectangle_print_format(text_rectangle, "+");
    }    
    
    // bottom scale
    if (show_scale) {
        text_rectangle_next_row(text_rectangle);
        level_map_print_scale_row(level_map->box, text_rectangle);
    }
    return text_rectangle;
}


void
level_map_calculate_text_rectangle_dimensions(struct size level_map_size,
                                              bool show_scale,
                                              int *column_count_out,
                                              int *row_count_out)
{
    int const left_scale_width = show_scale ? 4 : 0;
    int const text_columns_per_map_column = 4;
    int const right_border_width = 1;
    int const right_scale_width = show_scale ? 4 : 0;
    *column_count_out = left_scale_width
                      + level_map_size.width * text_columns_per_map_column
                      + right_border_width
                      + right_scale_width;

    int const top_scale_length = show_scale ? 1 : 0;
    int const top_border_length = 1;
    int const text_rows_per_map_row = 2;
    int const bottom_scale_length = show_scale ? 1 : 0;
    *row_count_out = top_scale_length
                   + top_border_length
                   + level_map_size.length * text_rows_per_map_row
                   + bottom_scale_length;
}


void
level_map_free(struct level_map *level_map)
{
    if (level_map) {
        free_or_die(level_map->tiles);
        free_or_die(level_map);
    }
}


struct tile *
level_map_tile_at(struct level_map const *level_map, struct point point)
{
    if(!box_contains_point(level_map->box, point)) return NULL;
    int index = box_index_for_point(level_map->box, point);
    return level_map->tiles[index];
}


void
level_map_print_border_row(struct size level_map_size,
                           struct text_rectangle *text_rectangle,
                           bool show_scale)
{
    if (show_scale) text_rectangle_print_format(text_rectangle, LMARGIN);
    for (int i = 0; i < level_map_size.width; ++i) {
        text_rectangle_print_format(text_rectangle, "+---");
    }
    text_rectangle_print_format(text_rectangle, "+   ");
}


void
level_map_print_scale_row(struct box level_map_box,
                          struct text_rectangle *text_rectangle)
{
    text_rectangle_print_format(text_rectangle, LMARGIN);
    for (int i = 0; i < level_map_box.size.width; ++i) {
        int x = level_map_box.origin.x + i;
        text_rectangle_print_format(text_rectangle, "%3i ", x);
    }
}


static void
tile_bottom_half(struct level_map const *level_map,
                 struct point point,
                 char half_tile[5])
{
    struct tile *tile = level_map_tile_at(level_map, point);
    if (wall_type_none == tile->walls.south) {
        fill_half_tile(tile, half_tile);
        if (tile_type_empty == tile->type) half_tile[0] = '.';
    }
    
    // south wall
    if (level_map->box.origin.y == point.y) {
        strcpy(half_tile, "----");
    } else if (wall_type_solid == tile->walls.south) {
        strcpy(half_tile, "----");
    } else if (wall_type_door == tile->walls.south) {
        strcpy(half_tile, "-[-]");
    } else if (wall_type_secret_door == tile->walls.south) {
        strcpy(half_tile, "--s-");
    }
    
    // west wall
    if (tile_has_west_wall(tile)) half_tile[0] = '|';
    
    // corner
    bool has_corner = false;
    if (level_map->box.origin.x == point.x) {
        has_corner = true;
    } else if (level_map->box.origin.y == point.y) {
        has_corner = true;
    } else if (!tile_has_south_wall(tile) && !tile_has_west_wall(tile)) {
        struct tile *south_tile = level_map_tile_at(level_map, point_south(point));
        struct tile *west_tile = level_map_tile_at(level_map, point_west(point));
        if (   south_tile
            && west_tile
            && (tile_has_west_wall(south_tile) || tile_has_south_wall(west_tile)))
        {
            has_corner = true;
        }
    } else if (tile_has_south_wall(tile) && !tile_has_west_wall(tile)) {
        struct tile *south_tile = level_map_tile_at(level_map, point_south(point));
        if (south_tile && tile_has_west_wall(south_tile)) {
            has_corner = true;
        } else {
            struct tile *west_tile = level_map_tile_at(level_map, point_west(point));
            if (west_tile && !tile_has_south_wall(west_tile)) {
                has_corner = true;
            }
        }
    } else if (!tile_has_south_wall(tile) && tile_has_west_wall(tile)) {
        struct tile *south_tile = level_map_tile_at(level_map, point_south(point));
        if (south_tile && !tile_has_west_wall(south_tile)) {
            has_corner = true;
        } else {            
            struct tile *west_tile = level_map_tile_at(level_map, point_west(point));
            if (west_tile && tile_has_south_wall(west_tile)) {
                has_corner = true;
            }
        }
    } else if (tile_has_south_wall(tile) && tile_has_west_wall(tile)) {
        has_corner = true;
    }

    if (has_corner) half_tile[0] = '+';
}


static void
tile_top_half(struct level_map const *level_map,
              struct point point,
              char half_tile[5])
{
    struct tile *tile = level_map_tile_at(level_map, point);
    fill_half_tile(tile, half_tile);
    
    // south wall only on bottom half
    
    // west wall
    if (level_map->box.origin.x == point.x) {
        half_tile[0] = '|';
    } else if (wall_type_solid == tile->walls.west) {
        half_tile[0] = '|';
    } else if (wall_type_door == tile->walls.west) {
        half_tile[0] = '|';
        half_tile[1] = ']';
    } else if (wall_type_secret_door == tile->walls.west) {
        half_tile[0] = '$';
    }
    
    // features
    if (tile->features & tile_features_chimney_down) {
        half_tile[2] = 'o';
    }
    if (tile->features & tile_features_chimney_up) {
        half_tile[1] = '(';
        half_tile[3] = ')';
    }
    if (tile->features & tile_features_chute_entrance) {
        half_tile[2] = '@';
    }
    if (tile->features & tile_features_chute_exit) {
        half_tile[2] = '*';
    }
    
    // east door
    struct tile *east_tile = level_map_tile_at(level_map, point_east(point));
    if (east_tile && wall_type_door == east_tile->walls.west) {
        half_tile[3] = '[';
    }
}
