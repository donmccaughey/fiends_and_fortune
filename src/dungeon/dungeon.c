#include "dungeon.h"

#include <base/base.h>

#include "area.h"
#include "dungeon_options.h"
#include "generator.h"
#include "level_map.h"
#include "text_rectangle.h"
#include "tile.h"


void
dungeon_add_area(struct dungeon *dungeon, struct area *area)
{
    int index = dungeon->areas_count;
    ++dungeon->areas_count;
    dungeon->areas = xreallocarray(dungeon->areas,
                                   dungeon->areas_count,
                                   sizeof(struct area *));
    dungeon->areas[index] = area;
}


struct dungeon *
dungeon_alloc(void)
{
    struct dungeon *dungeon = xcalloc(1, sizeof(struct dungeon));
    dungeon->areas = xcalloc(1, sizeof(struct area *));
    dungeon->tiles = xcalloc(1, sizeof(struct tile *));
    return dungeon;
}


struct tile **
dungeon_alloc_tiles_for_box(struct dungeon *dungeon, struct box box)
{
    int count = box_volume(box);
    struct tile **tiles = xcalloc(count, sizeof(struct tile *));
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


int
dungeon_ending_level(struct dungeon const *dungeon)
{
    if (!dungeon->tiles_count) return 0;
    
    int level = dungeon->tiles[0]->point.z;
    for (int i = 0; i < dungeon->tiles_count; ++i) {
        level = max_int(level, dungeon->tiles[i]->point.z);
    }
    return level;
}


void
dungeon_free(struct dungeon *dungeon)
{
    if (dungeon) {
        for (int i = 0; i < dungeon->areas_count; ++i) {
            area_free(dungeon->areas[i]);
        }
        free(dungeon->areas);
        for (int i = 0; i < dungeon->tiles_count; ++i) {
            tile_free(dungeon->tiles[i]);
        }
        free(dungeon->tiles);
        free(dungeon);
    }
}


void
dungeon_generate(struct dungeon *dungeon,
                 struct rnd *rnd,
                 struct dungeon_options *dungeon_options,
                 dungeon_progress_callback *progress_callback,
                 void *callback_user_data)
{
    struct generator *generator = generator_alloc(dungeon,
                                                  rnd,
                                                  dungeon_options,
                                                  progress_callback,
                                                  callback_user_data);
    generator_generate(generator);
    generator_free(generator);
}


void
dungeon_generate_small(struct dungeon *dungeon)
{
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon,
                                                  global_rnd,
                                                  dungeon_options,
                                                  NULL,
                                                  NULL);
    generator_generate_small(generator);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
}


bool
dungeon_is_box_excavated(struct dungeon *dungeon, struct box box)
{
    for (int i = 0; i < box.size.width; ++i) {
        for (int j = 0; j < box.size.length; ++j) {
            for (int k = 0; k < box.size.height; ++k) {
                struct point point = point_make(box.origin.x + i,
                                                box.origin.y + j,
                                                box.origin.z + k);
                struct tile *tile = dungeon_tile_at(dungeon, point);
                if (tile_is_escavated(tile)) return true;
            }
        }
    }
    return false;
}


int
dungeon_level_count(struct dungeon const *dungeon)
{
    if (!dungeon->tiles_count) return 0;
    return dungeon_ending_level(dungeon) - dungeon_starting_level(dungeon) + 1;
}


struct ptr_array *
dungeon_alloc_descriptions_of_entrances_and_exits_for_level(struct dungeon *dungeon, int level)
{
    struct ptr_array *descriptions = ptr_array_alloc();
    for (int i = 0; i < dungeon->areas_count; ++i) {
        struct area *area = dungeon->areas[i];
        if (level != area->box.origin.z) continue;
        if (area_is_level_transition(area)) {
            char *location = point_alloc_xy(area_center_point(area));
            char *area_description = area_alloc_description(area);
            char *description = str_alloc_formatted("%-12s %s", location, area_description);
            ptr_array_add(descriptions, description);
            free(location);
            free(area_description);
        }
    }
    return descriptions;
}


struct ptr_array *
dungeon_alloc_descriptions_of_chambers_and_rooms_for_level(struct dungeon *dungeon, int level)
{
    struct ptr_array *descriptions = ptr_array_alloc();
    for (int i = 0; i < dungeon->areas_count; ++i) {
        struct area *area = dungeon->areas[i];
        if (level != area->box.origin.z) continue;
        if (area_is_chamber_or_room(area)) {
            char *location = point_alloc_xy(area_center_point(area));
            char *area_description = area_alloc_description(area);
            char *description = str_alloc_formatted("%-12s %s", location, area_description);
            ptr_array_add(descriptions, description);
            free(location);
            free(area_description);
        }
    }
    return descriptions;
}


void
dungeon_print_areas_for_level(struct dungeon *dungeon, int level, FILE *out)
{
    fprintf(out, "  Entrances and Exits:\n");
    struct ptr_array *descriptions = dungeon_alloc_descriptions_of_entrances_and_exits_for_level(dungeon, level);
    for (int i = 0; i < descriptions->count; ++i) {
        fprintf(out, "    %s\n", (char *)descriptions->elements[i]);
    }
    ptr_array_clear(descriptions, free);
    ptr_array_free(descriptions);
    
    fprintf(out, "\n");
    fprintf(out, "  Chambers and Rooms:\n");
    descriptions = dungeon_alloc_descriptions_of_chambers_and_rooms_for_level(dungeon, level);
    for (int i = 0; i < descriptions->count; ++i) {
        fprintf(out, "    %s\n", (char *)descriptions->elements[i]);
    }
    ptr_array_clear(descriptions, free);
    ptr_array_free(descriptions);
}


struct text_rectangle *
dungeon_alloc_text_rectangle_for_level(struct dungeon *dungeon, int level)
{
    struct level_map *level_map = level_map_alloc(dungeon, level);
    struct text_rectangle *text_rectangle = level_map_alloc_text_rectangle(level_map, true);
    level_map_free(level_map);

    return text_rectangle;
}


void
dungeon_print_map_for_level(struct dungeon *dungeon, int level, FILE *out)
{
    struct level_map *level_map = level_map_alloc(dungeon, level);
    struct text_rectangle *text_rectangle = level_map_alloc_text_rectangle(level_map, true);
    fprintf(out, "%s", text_rectangle->chars);
    text_rectangle_free(text_rectangle);
    level_map_free(level_map);
}


int
dungeon_starting_level(struct dungeon const *dungeon)
{
    if (!dungeon->tiles_count) return 0;
    
    int level = dungeon->tiles[0]->point.z;
    for (int i = 0; i < dungeon->tiles_count; ++i) {
        level = min_int(level, dungeon->tiles[i]->point.z);
    }
    return level;
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
