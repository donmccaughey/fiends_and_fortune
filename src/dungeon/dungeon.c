#include "dungeon.h"

#include <assert.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/str.h"

#include "area.h"
#include "dungeon_generator.h"
#include "level_map.h"
#include "text_rectangle.h"
#include "tile.h"
#include "tiles.h"


struct area *
dungeon_add_area(struct dungeon *dungeon,
                 enum area_type area_type,
                 enum orientation orientation,
                 struct box box,
                 enum tile_type tile_type)
{
    int index = dungeon->areas_count;
    ++dungeon->areas_count;
    dungeon->areas = reallocarray_or_die(dungeon->areas,
                                         dungeon->areas_count,
                                         sizeof(struct area *));
    dungeon->areas[index] = area_alloc(dungeon,
                                       area_type,
                                       orientation,
                                       box,
                                       tile_type);
    return dungeon->areas[index];
}


struct tile *
dungeon_add_tile(struct dungeon *dungeon,
                 struct point point,
                 enum tile_type type)
{
    int index = dungeon->tiles_count;
    ++dungeon->tiles_count;
    dungeon->tiles = reallocarray_or_die(dungeon->tiles,
                                         dungeon->tiles_count,
                                         sizeof(struct tile *));
    dungeon->tiles[index] = tile_alloc(point, type);
    tiles_add_tile(dungeon->xtiles, dungeon->tiles[index]);
    return dungeon->tiles[index];
}


struct dungeon *
dungeon_alloc(void)
{
    struct dungeon *dungeon = calloc_or_die(1, sizeof(struct dungeon));
    dungeon->areas = calloc_or_die(1, sizeof(struct area *));
    dungeon->tiles = calloc_or_die(1, sizeof(struct tile *));
    dungeon->xtiles = tiles_alloc();
    return dungeon;
}


struct tile **
dungeon_alloc_tiles_for_box(struct dungeon *dungeon, struct box box)
{
    int count = box_volume(box);
    struct tile **tiles = calloc_or_die(count, sizeof(struct tile *));
    struct point end = box_end_point(box);
    for (int k = box.origin.z; k < end.z; ++k) {
        for (int j = box.origin.y; j < end.y; ++j) {
            for (int i = box.origin.x; i < end.x; ++i) {
                struct point point = point_make(i, j, k);
                int index = box_index_for_point(box, point);
                tiles[index] = dungeon_tile_at(dungeon, point);
            }
        }
    }
    return tiles;
}


void
dungeon_free(struct dungeon *dungeon)
{
    if (dungeon) {
        for (int i = 0; i < dungeon->areas_count; ++i) {
            area_free(dungeon->areas[i]);
        }
        free_or_die(dungeon->areas);
        /* TODO: enable when xtiles is gone
        for (int i = 0; i < dungeon->tiles_count; ++i) {
            tile_free(dungeon->tiles[i]);
        }
        */
        free_or_die(dungeon->tiles);
        tiles_free(dungeon->xtiles);
        free_or_die(dungeon);
    }
}


void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd)
{
    struct dungeon_generator *generator = dungeon_generator_alloc(dungeon, rnd);
    dungeon_generator_generate(generator);
    dungeon_generator_free(generator);
}


void
dungeon_generate_small(struct dungeon *dungeon)
{
    struct dungeon_generator *generator = dungeon_generator_alloc(dungeon, NULL);
    dungeon_generator_generate_small(generator);
    dungeon_generator_free(generator);
}


void
dungeon_print_level(struct dungeon *dungeon, int level, FILE *out)
{
    struct level_map *level_map = level_map_alloc(dungeon, level);
    struct text_rectangle *text_rectangle = level_map_alloc_text_graph(level_map);
    fprintf(out, "%s", text_rectangle->chars);
    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
}


struct box
dungeon_box_for_level(struct dungeon *dungeon, int level)
{
    struct box box = box_make(point_make(0, 0, level), size_make(0, 0, 1));
    for (size_t i = 0; i < dungeon->xtiles->count; ++i) {
        struct tile *tile = dungeon->xtiles->members[i];
        if (tile->point.z < level) continue;
        if (tile->point.z > level) break;
        box = box_extend_to_include_point(box, tile->point);
    }
    return box;
}


struct tile *
dungeon_tile_at(struct dungeon *dungeon, struct point point)
{
    struct tile *tile = tiles_find_tile_at(dungeon->xtiles, point);
    if (!tile) {
        tile = dungeon_add_tile(dungeon, point, tile_type_solid);
    }
    return tile;
}
