#include "dungeon.h"

#include <assert.h>
#include <string.h>

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/fail.h"
#include "common/rnd.h"
#include "common/str.h"

#include "area.h"
#include "generator.h"
#include "level_map.h"
#include "text_rectangle.h"
#include "tile.h"


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
                 enum tile_type tile_type)
{
    int index = dungeon->tiles_count;
    ++dungeon->tiles_count;
    dungeon->tiles = reallocarray_or_die(dungeon->tiles,
                                         dungeon->tiles_count,
                                         sizeof(struct tile *));
    struct tile *tile = tile_alloc(point, tile_type);
    dungeon->tiles[index] = tile;
    tile_sort_array_by_point(dungeon->tiles, dungeon->tiles_count);
    return tile;
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


void
dungeon_fill_box(struct dungeon *dungeon,
                 struct box box,
                 enum tile_type tile_type)
{
    struct box padded_box = box_expand(box, size_make(1, 1, 0));
    struct tile **tiles = dungeon_alloc_tiles_for_box(dungeon, padded_box);
    struct point point;
    for (int k = 0; k < padded_box.size.height; ++k) {
        point.z = padded_box.origin.z + k;
        for (int j = 1; j < padded_box.size.length; ++j) {
            point.y = padded_box.origin.y + j;
            for (int i = 1; i < padded_box.size.width; ++i) {
                point.x = padded_box.origin.x + i;
                int index = box_index_for_point(padded_box, point);
                struct tile *tile = tiles[index];
                if (box_contains_point(box, point)) tile->type = tile_type;
                
                int west_index = box_index_for_point(padded_box, point_west(point));
                struct tile *west_tile = tiles[west_index];
                if (west_tile->type != tile->type) {
                    tile->walls.west = wall_type_solid;
                }
                
                int south_index = box_index_for_point(padded_box, point_south(point));
                struct tile *south_tile = tiles[south_index];
                if (south_tile->type != tile->type) {
                    tile->walls.south = wall_type_solid;
                }
            }
        }
    }
    
    free_or_die(tiles);
}


bool
dungeon_is_box_excavated(struct dungeon *dungeon, struct box box)
{
    for (size_t i = 0; i < dungeon->tiles_count; ++i) {
        struct tile *tile = dungeon->tiles[i];
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


void
dungeon_set_wall(struct dungeon *dungeon,
                 struct point point,
                 enum direction direction,
                 enum wall_type wall_type)
{
    struct tile *tile;
    switch (direction) {
        case direction_north:
            tile = dungeon_tile_at(dungeon, point_north(point));
            tile->walls.south = wall_type;
            break;
        case direction_south:
            tile = dungeon_tile_at(dungeon, point);
            tile->walls.south = wall_type;
            break;
        case direction_east:
            tile = dungeon_tile_at(dungeon, point_east(point));
            tile->walls.west = wall_type;
            break;
        case direction_west:
            tile = dungeon_tile_at(dungeon, point);
            tile->walls.west = wall_type;
            break;
        default:
            fail("Unrecognized direction %i", direction);
            break;
    }
}


void
dungeon_set_walls(struct dungeon *dungeon,
                  struct box box,
                  enum wall_type wall_type)
{
    struct point end = box_end_point(box);
    
    for (int i = 0; i < box.size.width; ++i) {
        int x = box.origin.x + i;
        
        struct point point = point_make(x, box.origin.y, box.origin.z);
        dungeon_set_wall(dungeon, point, direction_south, wall_type);
        
        point = point_make(x, end.y, box.origin.z);
        dungeon_set_wall(dungeon, point, direction_south, wall_type);
    }
    
    for (int j = 0; j < box.size.length; ++j) {
        int y = box.origin.y + j;
        
        struct point point = point_make(box.origin.x, y, box.origin.z);
        dungeon_set_wall(dungeon, point, direction_west, wall_type);
        
        point = point_make(end.x, y, box.origin.z);
        dungeon_set_wall(dungeon, point, direction_west, wall_type);
    }
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
        return dungeon_add_tile(dungeon, point, tile_type_solid);
    }
}
