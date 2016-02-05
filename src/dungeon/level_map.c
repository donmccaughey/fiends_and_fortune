#include "level_map.h"

#include <assert.h>
#include <limits.h>
#include <stdarg.h>

#include "common/alloc_or_die.h"

#include "dungeon.h"
#include "range.h"
#include "reverse_range.h"
#include "text_rectangle.h"
#include "tile.h"
#include "tile_type.h"
#include "tiles.h"


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
print_border_row(struct level_map *level_map,
                 struct text_rectangle *text_rectangle,
                 int row_index);

static void
print_scale_row(struct level_map *level_map,
                struct text_rectangle *text_rectangle,
                int row_index);

static enum tile_type
tile_type_at(struct level_map *level_map, int32_t x, int32_t y);


static int
level_map_tile_index(struct level_map const *level_map, struct point point)
{
    int column = point.x - level_map->x_range.begin;
    int row = point.y - level_map->y_range.begin;
    int row_length = range_length(level_map->x_range);
    return row * row_length + column;
}


static uint32_t
level_map_tiles_count(struct level_map const *level_map)
{
    return range_length(level_map->x_range) * range_length(level_map->y_range);
}


struct level_map *
level_map_alloc(struct dungeon *dungeon, uint32_t level)
{
    struct level_map *level_map = calloc_or_die(1, sizeof(struct level_map));
    level_map->dungeon = dungeon;
    level_map->level = level;
    
    struct tiles *tiles_for_level = tiles_alloc_tiles_for_level(dungeon->tiles, level);
    level_map->x_range = range_expand(tiles_for_level->x_range, 1);
    level_map->y_range = range_expand(tiles_for_level->y_range, 1);
    
    uint32_t tiles_count = level_map_tiles_count(level_map);
    level_map->grid = calloc_or_die(tiles_count, sizeof(struct tile *));
    
    uint32_t count = 0;
    for (int32_t j = level_map->y_range.begin; j < level_map->y_range.end; ++j) {
        for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
            struct point point = point_make(i, j, level);
            struct tile *tile = tiles_find_tile_at(dungeon->tiles, point);
            if (!tile) tile = tile_alloc(point, tile_type_solid);
            int index = level_map_tile_index(level_map, point);
            level_map->grid[index] = tile;
            ++count;
        }
    }
    
    tiles_free(tiles_for_level);
    return level_map;
}


struct text_rectangle *
level_map_alloc_text_graph(struct level_map *level_map)
{
    int const top_scale_height = 1;
    int const top_border_height = 1;
    int const text_rows_per_map_row = 2;
    int const bottom_scale_height = 1;
    int row_count = top_scale_height
                  + top_border_height
                  + (range_length(level_map->y_range) * text_rows_per_map_row)
                  + bottom_scale_height;
    
    int const left_scale_width = 4;
    int const text_columns_per_map_column = 4;
    int const right_border_width = 1;
    int const right_scale_width = 4;
    int column_count = left_scale_width
                     + (range_length(level_map->x_range) * text_columns_per_map_column)
                     + right_border_width
                     + right_scale_width;
    
    struct text_rectangle *text_rectangle = text_rectangle_alloc(column_count,
                                                                 row_count);
    int row_index = 0;
    print_scale_row(level_map, text_rectangle, row_index);
    
    ++row_index;
    print_border_row(level_map, text_rectangle, row_index);
    
    // map tiles
    struct reverse_range y_reverse_range = reverse_range_from_range(level_map->y_range);
    for (int32_t j = y_reverse_range.top; j > y_reverse_range.bottom; --j) {
        // top line of row
        ++row_index;
        int column_index = 0;
        column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, LMARGIN_NUM, j);
        for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
            enum tile_type type = tile_type_at(level_map, i, j);
            enum tile_type west_type = tile_type_at(level_map, i - 1, j);
            char const *format;
            if (level_map->x_range.begin == i || type != west_type) {
                format = tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY;
            } else {
                format = tile_type_solid == type ? SOLID : EMPTY;
            }
            column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, format);
        }
        column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, RMARGIN_NUM, j);
        
        // bottom line of row
        ++row_index;
        column_index = 0;
        column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, LMARGIN);
        for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
            if (j == y_reverse_range.bottom + 1) {
                column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, CORNER_HWALL);
                continue;
            }
            
            enum tile_type type = tile_type_at(level_map, i, j);
            enum tile_type south_type = tile_type_at(level_map, i, j - 1);
            if (level_map->x_range.begin == i) {
                char const *format;
                if (type == south_type) {
                    format = tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY;
                } else {
                    format = CORNER_HWALL;
                }
                column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, format);
                continue;
            }
            
            enum tile_type west_type = tile_type_at(level_map, i - 1, j);
            enum tile_type south_west_type = tile_type_at(level_map, i - 1, j - 1);
            char const *format;
            if (type == south_type) {
                if (type == west_type) {
                    if (type == south_west_type) {
                        format = tile_type_solid == type ? SOLID : EMPTY_SPAN;
                    } else {
                        format = tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY;
                    }
                } else {
                    if (type == south_west_type) {
                        format = tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY;
                    } else {
                        format = tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY;
                    }
                }
            } else {
                if (type == west_type) {
                    if (type == south_west_type) {
                        format = CORNER_HWALL;
                    } else {
                        format = HWALL;
                    }
                } else {
                    format = CORNER_HWALL;
                }
            }
            column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, format);
        }
        text_rectangle_print_format_at(text_rectangle, column_index, row_index, "+");
    }    
    
    // bottom scale
    ++row_index;
    print_scale_row(level_map, text_rectangle, row_index++);
    return text_rectangle;
}


void
level_map_free(struct level_map *level_map)
{
    if (level_map) {
        uint32_t tiles_count = level_map_tiles_count(level_map);
        for (uint32_t i = 0; i < tiles_count; ++i) {
            if (tile_type_solid == level_map->grid[i]->type) {
                tile_free(level_map->grid[i]);
            }
        }
        free_or_die(level_map->grid);
        free_or_die(level_map);
    }
}


struct tile *
level_map_tile_at(struct level_map *level_map, uint32_t x, uint32_t y)
{
    assert(range_contains(level_map->x_range, x));
    assert(range_contains(level_map->y_range, y));
    struct point point = point_make(x, y, level_map->level);
    int index = level_map_tile_index(level_map, point);
    return level_map->grid[index];
}


static void
print_border_row(struct level_map *level_map,
                 struct text_rectangle *text_rectangle,
                 int row_index)
{
    int column_index = 0;
    column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, LMARGIN);
    for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
        column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, "+---");
    }
    text_rectangle_print_format_at(text_rectangle, column_index, row_index, "+   ");
}


static void
print_scale_row(struct level_map *level_map,
                struct text_rectangle *text_rectangle,
                int row_index)
{
    int column_index = 0;
    column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, LMARGIN);
    for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
        column_index += text_rectangle_print_format_at(text_rectangle, column_index, row_index, "%3i ", i);
    }
}


static enum tile_type
tile_type_at(struct level_map *level_map, int32_t x, int32_t y)
{
    if (   !range_contains(level_map->x_range, x)
        || !range_contains(level_map->y_range, y))
    {
        return tile_type_solid;
    }
    return level_map_tile_at(level_map, x, y)->type;
}
