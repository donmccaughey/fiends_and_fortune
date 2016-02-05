#include "level_map.h"

#include <assert.h>
#include <stdarg.h>

#include "common/alloc_or_die.h"

#include "dungeon.h"
#include "range.h"
#include "reverse_range.h"
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
print_border_row(struct level_map *level_map, struct lines_array *lines_array);

static char *
print_format(char *buffer, char const *format, ...);

static void
print_scale_row(struct level_map *level_map, struct lines_array *lines_array);

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


struct lines_array *
level_map_alloc_text_graph(struct level_map *level_map)
{
    int const left_scale_width = 4;
    int const text_columns_per_map_column = 4;
    int const right_border_width = 1;
    int const right_scale_width = 4;
    int const line_break_width = 1;
    int line_length = left_scale_width
                    + range_length(level_map->x_range) * text_columns_per_map_column
                    + right_border_width
                    + right_scale_width
                    + line_break_width;
    
    struct lines_array *lines_array = lines_array_alloc(line_length);
    print_scale_row(level_map, lines_array);
    print_border_row(level_map, lines_array);
    
    // map tiles
    struct reverse_range y_reverse_range = reverse_range_from_range(level_map->y_range);
    for (int32_t j = y_reverse_range.top; j > y_reverse_range.bottom; --j) {
        // top line of row
        char *row = lines_array_add_line(lines_array);
        row = print_format(row, LMARGIN_NUM, j);
        for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
            enum tile_type type = tile_type_at(level_map, i, j);
            enum tile_type west_type = tile_type_at(level_map, i - 1, j);
            if (level_map->x_range.begin == i || type != west_type) {
                row = stpcpy(row, tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY);
            } else {
                row = stpcpy(row, tile_type_solid == type ? SOLID : EMPTY);
            }
        }
        print_format(row, RMARGIN_NUM, j);
        
        
        // bottom line of row
        row = lines_array_add_line(lines_array);
        row = stpcpy(row, LMARGIN);
        for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
            if (j == y_reverse_range.bottom + 1) {
                row = stpcpy(row, CORNER_HWALL);
                continue;
            }
            
            enum tile_type type = tile_type_at(level_map, i, j);
            enum tile_type south_type = tile_type_at(level_map, i, j - 1);
            if (level_map->x_range.begin == i) {
                if (type == south_type) {
                    row = stpcpy(row, tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY);
                } else {
                    row = stpcpy(row, CORNER_HWALL);
                }
                continue;
            }
            
            enum tile_type west_type = tile_type_at(level_map, i - 1, j);
            enum tile_type south_west_type = tile_type_at(level_map, i - 1, j - 1);
            if (type == south_type) {
                if (type == west_type) {
                    if (type == south_west_type) {
                        row = stpcpy(row, tile_type_solid == type ? SOLID : EMPTY_SPAN);
                    } else {
                        row = stpcpy(row, tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY);
                    }
                } else {
                    if (type == south_west_type) {
                        row = stpcpy(row, tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY);
                    } else {
                        row = stpcpy(row, tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY);
                    }
                }
            } else {
                if (type == west_type) {
                    if (type == south_west_type) {
                        row = stpcpy(row, CORNER_HWALL);
                    } else {
                        row = stpcpy(row, HWALL);
                    }
                } else {
                    row = stpcpy(row, CORNER_HWALL);
                }
            }
        }
        row = stpcpy(row, "+\n");
    }    
    
    // bottom scale
    print_scale_row(level_map, lines_array);
    return lines_array;
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


char *
lines_array_add_line(struct lines_array *lines_array)
{
    int index = lines_array->lines_count;
    ++lines_array->lines_count;
    lines_array->lines = reallocarray_or_die(lines_array->lines,
                                             lines_array->lines_count,
                                             sizeof(char *));
    lines_array->lines[index] = calloc_or_die(lines_array->line_length + 1,
                                              sizeof(char));
    return lines_array->lines[index];
}


struct lines_array *
lines_array_alloc(int line_length)
{
    struct lines_array *lines_array = calloc_or_die(1, sizeof(struct lines_array));
    lines_array->lines = calloc_or_die(1, sizeof(char *));
    lines_array->line_length = line_length;
    return lines_array;
}


void
lines_array_free(struct lines_array *lines_array)
{
    if (lines_array) {
        for (int i = 0; i < lines_array->lines_count; ++i) {
            free_or_die(lines_array->lines[i]);
        }
        free_or_die(lines_array->lines);
        free_or_die(lines_array);
    }
}


static void
print_border_row(struct level_map *level_map, struct lines_array *lines_array)
{
    char *row = lines_array_add_line(lines_array);
    row = stpcpy(row, LMARGIN);
    for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
        row = stpcpy(row, "+---");
    }
    stpcpy(row, "+   \n");
}


static char *
print_format(char *buffer, char const *format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    int chars_printed = vsprintf(buffer, format, arguments);
    assert(chars_printed >= 0);
    va_end(arguments);
    return buffer + chars_printed;
}


static void
print_scale_row(struct level_map *level_map, struct lines_array *lines_array)
{
    char *row = lines_array_add_line(lines_array);
    row = stpcpy(row, LMARGIN);
    for (int32_t i = level_map->x_range.begin; i < level_map->x_range.end; ++i) {
        row = print_format(row, "%3i ", i);
    }
    stpcpy(row, RMARGIN "\n");
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
