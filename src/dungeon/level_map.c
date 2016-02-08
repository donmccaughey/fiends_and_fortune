#include "level_map.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "dungeon.h"
#include "text_rectangle.h"
#include "tile.h"
#include "tile_type.h"
#include "wall_type.h"


#define CORNER_EMPTY "+   "
#define CORNER_HWALL "+---"
#define CORNER_SOLID "+:::"
#define EMPTY "    "
#define EMPTY_SPAN ".   "
#define HWALL "----"
#define SOLID "::::"
#define VWALL_EMPTY "|   "
#define VWALL_SOLID "|:::"

#define LMARGIN "    "
#define LMARGIN_NUM "%3i "
#define RMARGIN "    "
#define RMARGIN_NUM "| %-3i\n"


static void
print_border_row(struct level_map const *level_map,
                 struct text_rectangle *text_rectangle);

static void
print_scale_row(struct level_map const *level_map,
                struct text_rectangle *text_rectangle);

static char const *
tile_bottom_half(struct level_map const *level_map, struct point point);

static char const *
tile_top_half(struct level_map const *level_map, struct point point);


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
level_map_alloc_text_graph(struct level_map *level_map)
{
    int const top_scale_length = 1;
    int const top_border_length = 1;
    int const text_rows_per_map_row = 2;
    int const bottom_scale_length = 1;
    int row_count = top_scale_length
                  + top_border_length
                  + level_map->box.size.length * text_rows_per_map_row
                  + bottom_scale_length;
    
    int const left_scale_width = 4;
    int const text_columns_per_map_column = 4;
    int const right_border_width = 1;
    int const right_scale_width = 4;
    int column_count = left_scale_width
                     + level_map->box.size.width * text_columns_per_map_column
                     + right_border_width
                     + right_scale_width;
    
    struct text_rectangle *text_rectangle = text_rectangle_alloc(column_count,
                                                                 row_count);
    print_scale_row(level_map, text_rectangle);
    
    text_rectangle_next_row(text_rectangle);
    print_border_row(level_map, text_rectangle);
    
    // map tiles
    struct point point = level_map->box.origin;
    point.y += level_map->box.size.length;
    for (int j = 0; j < level_map->box.size.length; ++j) {
        --point.y;
        // top line of row
        text_rectangle_next_row(text_rectangle);
        text_rectangle_print_format(text_rectangle, LMARGIN_NUM, point.y);
        point.x = level_map->box.origin.x;
        for (int i = 0; i < level_map->box.size.width; ++i) {
            text_rectangle_print_format(text_rectangle,
                                        tile_top_half(level_map, point));
            ++point.x;
        }
        text_rectangle_print_format(text_rectangle, RMARGIN_NUM, point.y);
        
        // bottom line of row
        text_rectangle_next_row(text_rectangle);
        text_rectangle_print_format(text_rectangle, LMARGIN);
        point.x = level_map->box.origin.x;
        for (int i = 0; i < level_map->box.size.width; ++i) {
            text_rectangle_print_format(text_rectangle,
                                        tile_bottom_half(level_map, point));
            ++point.x;
        }
        text_rectangle_print_format(text_rectangle, "+");
    }    
    
    // bottom scale
    text_rectangle_next_row(text_rectangle);
    print_scale_row(level_map, text_rectangle);
    return text_rectangle;
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


static void
print_border_row(struct level_map const *level_map,
                 struct text_rectangle *text_rectangle)
{
    text_rectangle_print_format(text_rectangle, LMARGIN);
    for (int i = 0; i < level_map->box.size.width; ++i) {
        text_rectangle_print_format(text_rectangle, "+---");
    }
    text_rectangle_print_format(text_rectangle, "+   ");
}


static void
print_scale_row(struct level_map const *level_map,
                struct text_rectangle *text_rectangle)
{
    text_rectangle_print_format(text_rectangle, LMARGIN);
    for (int i = 0; i < level_map->box.size.width; ++i) {
        int x = level_map->box.origin.x + i;
        text_rectangle_print_format(text_rectangle, "%3i ", x);
    }
}


static char const *
tile_bottom_half(struct level_map const *level_map, struct point point)
{
    struct tile *tile = level_map_tile_at(level_map, point);
    if (level_map->box.origin.y == point.y) return CORNER_HWALL;
    if (level_map->box.origin.x == point.x) {
        if (tile_type_solid == tile->type) return CORNER_SOLID;
        if (tile_type_empty == tile->type) return CORNER_EMPTY;
    }
    
    if (   wall_type_none == tile->walls.west
        && wall_type_none == tile->walls.south)
    {
        struct tile *tile_west = level_map_tile_at(level_map, point_west(point));
        struct tile *tile_south = level_map_tile_at(level_map, point_south(point));
        if (   tile_west
            && tile_south
            && wall_type_none != tile_west->walls.south
            && wall_type_none != tile_south->walls.west)
        {
            if (tile_type_solid == tile->type) return CORNER_SOLID;
            if (tile_type_empty == tile->type) return CORNER_EMPTY;
        } else {
            if (tile_type_solid == tile->type) return SOLID;
            if (tile_type_empty == tile->type) return EMPTY_SPAN;
        }
    }
    
    if (   wall_type_solid == tile->walls.west
        && wall_type_solid == tile->walls.south)
    {
        return CORNER_HWALL;
    }
    
    if (   wall_type_solid == tile->walls.west
        && wall_type_none == tile->walls.south)
    {
        struct tile *tile_west = level_map_tile_at(level_map, point_west(point));
        if (tile_west && wall_type_none != tile_west->walls.south) {
            if (tile_type_solid == tile->type) return CORNER_SOLID;
            if (tile_type_empty == tile->type) return CORNER_EMPTY;
        } else {
            if (tile_type_solid == tile->type) return VWALL_SOLID;
            if (tile_type_empty == tile->type) return VWALL_EMPTY;
        }
    }
    
    if (   wall_type_none == tile->walls.west
        && wall_type_solid == tile->walls.south)
    {
        struct tile *tile_south = level_map_tile_at(level_map, point_south(point));
        if (tile_south && wall_type_none != tile_south->walls.west) {
            return CORNER_HWALL;
        } else {
            return HWALL;
        }
    }
    
    return SOLID;
}


static char const *
tile_top_half(struct level_map const *level_map, struct point point)
{
    struct tile *tile = level_map_tile_at(level_map, point);
    if (level_map->box.origin.x == point.x) {
        if (tile_type_solid == tile->type) return VWALL_SOLID;
        if (tile_type_empty == tile->type) return VWALL_EMPTY;
    }
    if (tile_type_empty == tile->type) {
        if (wall_type_none == tile->walls.west) return EMPTY;
        if (wall_type_solid == tile->walls.west) return VWALL_EMPTY;
    }
    if (tile_type_solid == tile->type) {
        if (wall_type_none == tile->walls.west) return SOLID;
        if (wall_type_solid == tile->walls.west) return VWALL_SOLID;
    }
    return SOLID;
}
