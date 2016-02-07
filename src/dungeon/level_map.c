#include "level_map.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "dungeon.h"
#include "text_rectangle.h"
#include "tile.h"
#include "tile_type.h"


#define CORNER_EMPTY "+   "
#define CORNER_HWALL "+---"
#define CORNER_SOLID "+:::"
#define EMPTY "    "
#define EMPTY_SPAN ".   "
#define HWALL "----"
#define LMARGIN "    "
#define LMARGIN_NUM "%3i "
#define RMARGIN "    "
#define RMARGIN_NUM "| %-3i\n"
#define SOLID "::::"
#define VWALL_EMPTY "|   "
#define VWALL_SOLID "|:::"


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

static enum tile_type
tile_type_at(struct level_map const *level_map, struct point point);


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
    assert(box_contains_point(level_map->box, point));
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
    if (point.y == level_map->box.origin.y) return CORNER_HWALL;
    
    enum tile_type type = tile_type_at(level_map, point);
    enum tile_type south_type = tile_type_at(level_map, point_south(point));
    
    if (level_map->box.origin.x == point.x) {
        if (type == south_type) {
            return tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY;
        } else {
            return CORNER_HWALL;
        }
    }
    
    enum tile_type west_type = tile_type_at(level_map, point_west(point));
    enum tile_type south_west_type = tile_type_at(level_map, point_south_west(point));
    
    if (type == south_type) {
        if (type == west_type) {
            if (type == south_west_type) {
                return tile_type_solid == type ? SOLID : EMPTY_SPAN;
            } else {
                return tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY;
            }
        } else {
            if (type == south_west_type) {
                return tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY;
            } else {
                return tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY;
            }
        }
    }
    
    if (type == west_type) {
        if (type == south_west_type) {
            return CORNER_HWALL;
        } else {
            return HWALL;
        }
    }
    
    return CORNER_HWALL;
}


static char const *
tile_top_half(struct level_map const *level_map, struct point point)
{
    enum tile_type type = tile_type_at(level_map, point);
    enum tile_type west_type = tile_type_at(level_map, point_west(point));
    
    if (level_map->box.origin.x == point.x || type != west_type) {
        return tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY;
    } else {
        return tile_type_solid == type ? SOLID : EMPTY;
    }
}


static enum tile_type
tile_type_at(struct level_map const *level_map, struct point point)
{
    if (!box_contains_point(level_map->box, point)) return tile_type_solid;
    return level_map_tile_at(level_map, point)->type;
}
