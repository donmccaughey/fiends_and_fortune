#include "text_graph.h"

#include "dungeon.h"
#include "range.h"
#include "reverse_range.h"
#include "tile.h"
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
print_horizontal_border(FILE *out, struct range range);

static void
print_horizontal_scale(FILE *out, struct range range);

static enum tile_type
tile_type_at(struct tiles const *tiles, int32_t x, int32_t y, int32_t z);


void
dungeon_graph_level_using_text(struct dungeon *dungeon,
                               int32_t level,
                               FILE *out)
{
    struct tiles *level_tiles = tiles_alloc_with_parent_and_level(dungeon->tiles, level);
    struct range x_range = range_expand(tiles_x_range(level_tiles), 1);
    struct range y_range = range_expand(tiles_y_range(level_tiles), 1);
    struct reverse_range y_reverse_range = reverse_range_from_range(y_range);
    
    // top border
    print_horizontal_scale(out, x_range);
    print_horizontal_border(out, x_range);
    
    // rows of tiles
    for (int32_t j = y_reverse_range.top; j > y_reverse_range.bottom; --j) {
        // top line of row
        fprintf(out, LMARGIN_NUM, j);
        for (int32_t i = x_range.begin; i < x_range.end; ++i) {
            enum tile_type type = tile_type_at(level_tiles, i, j, level);
            enum tile_type west_type = tile_type_at(level_tiles, i - 1, j, level);
            if (x_range.begin == i || type != west_type) {
                fprintf(out, tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY);
            } else {
                fprintf(out, tile_type_solid == type ? SOLID : EMPTY);
            }
        }
        fprintf(out, RMARGIN_NUM, j);
        
        // bottom line of row
        fprintf(out, LMARGIN);
        for (int32_t i = x_range.begin; i < x_range.end; ++i) {
            if (j == y_reverse_range.bottom + 1) {
                fprintf(out, CORNER_HWALL);
                continue;
            }
            
            enum tile_type type = tile_type_at(level_tiles, i, j, level);
            enum tile_type south_type = tile_type_at(level_tiles, i, j - 1, level);
            if (x_range.begin == i) {
                if (type == south_type) {
                    fprintf(out, tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY);
                } else {
                    fprintf(out, CORNER_HWALL);
                }
                continue;
            }
            
            enum tile_type west_type = tile_type_at(level_tiles, i - 1, j, level);
            enum tile_type south_west_type = tile_type_at(level_tiles, i - 1, j - 1, level);
            if (type == south_type) {
                if (type == west_type) {
                    if (type == south_west_type) {
                        fprintf(out, tile_type_solid == type ? SOLID : EMPTY_SPAN);
                    } else {
                        fprintf(out, tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY);
                    }
                } else {
                    if (type == south_west_type) {
                        fprintf(out, tile_type_solid == type ? CORNER_SOLID : CORNER_EMPTY);
                    } else {
                        fprintf(out, tile_type_solid == type ? VWALL_SOLID : VWALL_EMPTY);
                    }
                }
            } else {
                if (type == west_type) {
                    if (type == south_west_type) {
                        fprintf(out, CORNER_HWALL);
                    } else {
                        fprintf(out, HWALL);
                    }
                } else {
                    fprintf(out, CORNER_HWALL);
                }
            }
        }
        fprintf(out, "+\n");
    }
    
    print_horizontal_scale(out, x_range);
    tiles_free(level_tiles);
}


static void
print_horizontal_border(FILE *out, struct range range)
{
    fprintf(out, LMARGIN);
    for (int32_t i = range.begin; i < range.end; ++i) {
        fprintf(out, "+---");
    }
    fprintf(out, "+   \n");
}


static void
print_horizontal_scale(FILE *out, struct range range)
{
    fprintf(out, LMARGIN);
    for (int32_t i = range.begin; i < range.end; ++i) {
        fprintf(out, "%3i ", i);
    }
    fprintf(out, RMARGIN "\n");
}


static enum tile_type
tile_type_at(struct tiles const *tiles, int32_t x, int32_t y, int32_t z)
{
    struct tile *tile = tiles_find_tile_at(tiles, point_make(x, y, z));
    return tile ? tile->type : tile_type_solid;
}
