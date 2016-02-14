#include "dungeon.h"

#include "common/alloc_or_die.h"
#include "common/rnd.h"

#include "area.h"
#include "generator.h"
#include "level_map.h"
#include "text_rectangle.h"
#include "tile.h"


void
dungeon_add_area(struct dungeon *dungeon, struct area *area)
{
    int index = dungeon->areas_count;
    ++dungeon->areas_count;
    dungeon->areas = reallocarray_or_die(dungeon->areas,
                                         dungeon->areas_count,
                                         sizeof(struct area *));
    dungeon->areas[index] = area;
}


struct dungeon *
dungeon_alloc(void)
{
    struct dungeon *dungeon = calloc_or_die(1, sizeof(struct dungeon));
    dungeon->areas = calloc_or_die(1, sizeof(struct area *));
    dungeon->tiles = calloc_or_die(1, sizeof(struct tile *));
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


struct box
dungeon_box_for_level(struct dungeon *dungeon, int level)
{
    struct box box = box_make(point_make(0, 0, level), size_make(0, 0, 1));
    for (size_t i = 0; i < dungeon->tiles_count; ++i) {
        struct tile *tile = dungeon->tiles[i];
        if (tile->point.z < level) continue;
        if (tile->point.z > level) break;
        if (tile_is_unescavated(tile)) continue;
        box = box_extend_to_include_point(box, tile->point);
    }
    return box;
}


void
dungeon_free(struct dungeon *dungeon)
{
    if (dungeon) {
        for (int i = 0; i < dungeon->areas_count; ++i) {
            area_free(dungeon->areas[i]);
        }
        free_or_die(dungeon->areas);
        for (int i = 0; i < dungeon->tiles_count; ++i) {
            tile_free(dungeon->tiles[i]);
        }
        free_or_die(dungeon->tiles);
        free_or_die(dungeon);
    }
}


void
dungeon_generate(struct dungeon *dungeon, struct rnd *rnd)
{
    struct generator *generator = generator_alloc(dungeon, rnd);
    generator_generate(generator);
    generator_free(generator);
}


void
dungeon_generate_small(struct dungeon *dungeon)
{
    struct generator *generator = generator_alloc(dungeon, global_rnd);
    generator_generate_small(generator);
    generator_free(generator);
}


bool
dungeon_is_box_excavated(struct dungeon *dungeon, struct box box)
{
    for (size_t i = 0; i < dungeon->tiles_count; ++i) {
        struct tile *tile = dungeon->tiles[i];
        if (tile->point.z < box.origin.z) continue;
        if (tile->point.z >= box.origin.z + box.size.height) break;
        if (tile_is_unescavated(tile)) continue;
        if (box_contains_point(box, tile->point)) return true;
    }
    return false;
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


struct tile *
dungeon_tile_at(struct dungeon *dungeon, struct point point)
{
    struct tile **tile = tile_find_in_array_sorted_by_point(dungeon->tiles,
                                                            dungeon->tiles_count,
                                                            point);
    if (tile) {
        return *tile;
    } else {
        struct tile *default_tile = tile_alloc(point, tile_type_filled);
        dungeon->tiles = tile_add_to_array_sorted_by_point(dungeon->tiles,
                                                           &dungeon->tiles_count,
                                                           default_tile);
        return default_tile;
    }
}
