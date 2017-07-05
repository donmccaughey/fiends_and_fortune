#include "generator.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/fail.h"
#include "common/rnd.h"

#include "area.h"
#include "digger.h"
#include "dungeon.h"
#include "periodic_check.h"
#include "tile.h"


static struct digger *
digger_alloc(struct generator *generator,
             struct point point,
             enum direction direction)
{
    struct digger *digger = calloc_or_die(1, sizeof(struct digger));
    digger->generator = generator;
    digger->point = point;
    digger->direction = direction;
    return digger;
}


static void
digger_free(struct digger *digger)
{
    free_or_die(digger);
}


struct area *
generator_add_area(struct generator *generator,
                   enum area_type area_type,
                   enum direction direction,
                   struct box box,
                   enum tile_type tile_type)
{
    struct area *area = area_alloc(area_type, direction, box, tile_type);    
    int index = generator->areas_count;
    ++generator->areas_count;
    generator->areas = reallocarray_or_die(generator->areas,
                                         generator->areas_count,
                                         sizeof(struct area *));
    generator->areas[index] = area;
    generator_fill_box(generator, box, direction, tile_type);
    generator_set_walls(generator, box, wall_type_solid);
    return area;
}


struct digger *
generator_add_digger(struct generator *generator,
                     struct point point,
                     enum direction direction)
{
    int index = generator->diggers_count;
    ++generator->diggers_count;
    generator->diggers = reallocarray_or_die(generator->diggers,
                                             generator->diggers_count,
                                             sizeof(struct digger *));
    generator->diggers[index] = digger_alloc(generator, point, direction);
    return generator->diggers[index];
}


struct generator *
generator_alloc(struct dungeon *dungeon,
                struct rnd *rnd,
                generator_progress_callback *progress_callback,
                void *callback_user_data)
{
    struct generator *generator = calloc_or_die(1, sizeof(struct generator));
    generator->dungeon = dungeon;
    generator->rnd = rnd;
    
    generator->padding = rnd_next_uniform_value(rnd, 2);
    generator->max_iteration_count = 100;
    generator->max_size = size_make(30, 20, 5);
    
    generator->areas = calloc_or_die(1, sizeof(struct area *));
    generator->diggers = calloc_or_die(1, sizeof(struct digger *));
    generator->saved_diggers = calloc_or_die(1, sizeof(struct digger));
    generator->tiles = calloc_or_die(1, sizeof(struct tile *));
    
    generator->progress_callback = progress_callback;
    generator->callback_user_data = callback_user_data;
    
    return generator;
}


struct box
generator_box_for_level(struct generator *generator, int level)
{
    struct box box = box_make(point_make(0, 0, level), size_make(0, 0, 1));
    for (size_t i = 0; i < generator->tiles_count; ++i) {
        struct tile *tile = generator->tiles[i];
        if (level != tile->point.z) continue;
        if (tile_is_escavated(tile)) {
            box = box_extend_to_include_point(box, tile->point);
        }
    }
    for (size_t i = 0; i < generator->dungeon->tiles_count; ++i) {
        struct tile *dungeon_tile = generator->dungeon->tiles[i];
        if (level != dungeon_tile->point.z) continue;
        struct tile *tile = generator_tile_at(generator, dungeon_tile->point);
        if (tile_is_escavated(tile)) {
            box = box_extend_to_include_point(box, tile->point);
        }
    }
    return box;
}


void
generator_commit(struct generator *generator)
{
    for (int i = 0; i < generator->areas_count; ++i) {
        dungeon_add_area(generator->dungeon, generator->areas[i]);
    }
    generator->areas_count = 0;
    
    generator->saved_diggers_count = generator->diggers_count;
    generator->saved_diggers = reallocarray_or_die(generator->saved_diggers,
                                                   generator->saved_diggers_count,
                                                   sizeof(struct digger));
    for (int i = 0; i < generator->diggers_count; ++i) {
        generator->saved_diggers[i] = *(generator->diggers[i]);
    }
    
    for (int i = 0; i < generator->tiles_count; ++i) {
        struct tile *tile = dungeon_tile_at(generator->dungeon,
                                            generator->tiles[i]->point);
        *tile = *generator->tiles[i];
    }
    for (int i = 0; i < generator->tiles_count; ++i) {
        tile_free(generator->tiles[i]);
    }
    generator->tiles_count = 0;
}


struct digger *
generator_copy_digger(struct generator *generator, struct digger *digger)
{
    return generator_add_digger(generator, digger->point, digger->direction);
}


void
generator_delete_digger(struct generator *generator, struct digger *digger)
{
    int index = -1;
    for (int i = 0; i < generator->diggers_count; ++i) {
        if (digger == generator->diggers[i]) {
            index = i;
            break;
        }
    }
    assert(index >= 0 && index < generator->diggers_count);
    digger_free(digger);
    
    int next_index = index + 1;
    int last_index = generator->diggers_count - 1;
    if (next_index <= last_index) {
        struct digger **vacant = &generator->diggers[index];
        struct digger **next = &generator->diggers[next_index];
        size_t size = (last_index - index) * sizeof(struct digger *);
        memmove(vacant, next, size);
    }
    --generator->diggers_count;
    generator->diggers = reallocarray_or_die(generator->diggers,
                                             generator->diggers_count,
                                             sizeof(struct digger *));
}


void
generator_fill_box(struct generator *generator,
                   struct box box,
                   enum direction direction,
                   enum tile_type tile_type)
{
    struct box padded_box = box_expand(box, size_make(1, 1, 0));
    struct point point;
    for (int k = 0; k < padded_box.size.height; ++k) {
        point.z = padded_box.origin.z + k;
        for (int j = 1; j < padded_box.size.length; ++j) {
            point.y = padded_box.origin.y + j;
            for (int i = 1; i < padded_box.size.width; ++i) {
                point.x = padded_box.origin.x + i;
                struct tile *tile = generator_tile_at(generator, point);
                if (box_contains_point(box, point)) {
                    tile->direction = direction;
                    tile->type = tile_type;
                }
                
                struct tile *west_tile = generator_tile_at(generator, point_west(point));
                if (tile_is_escavated(west_tile) != tile_is_escavated(tile)) {
                    tile->walls.west = wall_type_solid;
                }
                
                struct tile *south_tile = generator_tile_at(generator, point_south(point));
                if (tile_is_escavated(south_tile) != tile_is_escavated(tile)) {
                    tile->walls.south = wall_type_solid;
                }
            }
        }
    }
}


void
generator_free(struct generator *generator)
{
    generator_rollback(generator);
    free_or_die(generator->areas);
    for (int i = 0; i < generator->diggers_count; ++i) {
        digger_free(generator->diggers[i]);
    }
    free_or_die(generator->diggers);
    free_or_die(generator->saved_diggers);
    free_or_die(generator->tiles);
    free_or_die(generator);
}


void
generator_generate(struct generator *generator)
{
    struct digger *digger = generator_add_digger(generator,
                                                 point_make(0, 0, 1),
                                                 direction_north);
    digger_dig_starting_stairs(digger);
    digger_dig_passage(digger, 1, wall_type_none);
    generator_commit(generator);
    
    while (   generator->diggers_count
           && generator->iteration_count < generator->max_iteration_count)
    {
        struct digger **diggers = arraydup_or_die(generator->diggers,
                                                  generator->diggers_count,
                                                  sizeof(struct digger *));
        int count = generator->diggers_count;
        for (int i = 0; i < count; ++i) {
            if (periodic_check(diggers[i])) {
                generator_commit(generator);
            } else {
                generator_rollback(generator);
            }
        }
        free_or_die(diggers);
        ++generator->iteration_count;
        if (generator->progress_callback) {
            generator->progress_callback(generator,
                                         generator->callback_user_data);
        }
    }
}


void
generator_generate_small(struct generator *generator)
{
    struct digger *digger = generator_add_digger(generator,
                                                 point_make(0, 0, 1),
                                                 direction_north);
    
    digger_dig_starting_stairs(digger);
    digger_dig_chamber(digger, 5, 3, 1, wall_type_none);
    
    // from entry chamber, north west exit
    struct digger *nw_digger = generator_copy_digger(generator, digger);
    digger_move_forward(nw_digger, 4);
    digger_turn_90_degrees_left(nw_digger);
    digger_move_forward(nw_digger, 1);
    digger_dig_passage(nw_digger, 2, wall_type_none);
    digger_turn_90_degrees_right(nw_digger);
    digger_dig_passage(nw_digger, 6, wall_type_none);
    digger_dig_chamber(nw_digger, 3, 2, 1, wall_type_none);
    
    // from entry chamber, south west exit
    struct digger *sw_digger = generator_copy_digger(generator, digger);
    digger_move_forward(sw_digger, 2);
    digger_turn_90_degrees_left(sw_digger);
    digger_move_forward(sw_digger, 1);
    digger_dig_passage(sw_digger, 2, wall_type_door);
    digger_turn_90_degrees_left(sw_digger);
    digger_dig_passage(sw_digger, 2, wall_type_none);
    digger_turn_90_degrees_right(sw_digger);
    digger_dig_passage(sw_digger, 2, wall_type_none);
    digger_turn_90_degrees_right(sw_digger);
    digger_dig_passage(sw_digger, 2, wall_type_door);
    digger_dig_chamber(sw_digger, 2, 2, 1, wall_type_none);
    digger_move_forward(sw_digger, 2);
    digger_move(sw_digger, 1, direction_west);
    digger_dig_passage(sw_digger, 3, wall_type_door);
    digger_dig_chamber(sw_digger, 2, 3, 1, wall_type_none);
    
    // from entry chamber, south east exit
    struct digger *se_digger = generator_copy_digger(generator, digger);
    digger_move_forward(se_digger, 2);
    digger_turn_90_degrees_right(se_digger);
    digger_move_forward(se_digger, 1);
    digger_dig_passage(se_digger, 1, wall_type_none);
    digger_dig_chamber(se_digger, 6, 4, 0, wall_type_none);
    // fill in one tile in chamber
    struct box box = box_make(point_make(5, 2, 1), size_make_unit());
    generator_fill_box(generator, box, direction_north, tile_type_filled);
    
    // from entry chamber, north exit
    digger_move_forward(digger, 5);
    digger_dig_passage(digger, 8, wall_type_none);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 8, wall_type_none);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 3, wall_type_none);
    digger_dig_chamber(digger, 3, 4, 0, wall_type_none);
    digger_move_forward(digger, 3);
    digger_move(digger, 3, direction_west);
    digger_dig_passage(digger, 3, wall_type_none);
    digger_turn_90_degrees_right(digger);
    // dig connecting passage without constraints to make looping passage
    digger_dig_area(digger, 3, 1, 0, wall_type_none, area_type_passage);
    digger_move_forward(digger, 3);
    struct tile *tile = generator_tile_at(generator, point_east(digger->point));
    tile->walls.west = wall_type_none;
    generator_commit(generator);
}


bool
generator_is_box_excavated(struct generator *generator, struct box box)
{
    for (int i = 0; i < box.size.width; ++i) {
        for (int j = 0; j < box.size.length; ++j) {
            for (int k = 0; k < box.size.height; ++k) {
                struct point point = point_make(box.origin.x + i,
                                                box.origin.y + j,
                                                box.origin.z + k);
                struct tile *tile = generator_tile_at(generator, point);
                if (tile_is_escavated(tile)) return true;
            }
        }
    }
    return false;
}


int
generator_max_level(struct generator const *generator)
{
    return generator->max_size.height;
}


int
generator_min_level(struct generator const *generator)
{
    return 1;
}


void
generator_rollback(struct generator *generator)
{
    for (int i = 0; i < generator->areas_count; ++i) {
        area_free(generator->areas[i]);
    }
    generator->areas_count = 0;
    
    while (generator->diggers_count < generator->saved_diggers_count) {
        generator_add_digger(generator, point_make(0, 0, 0), direction_north);
    }
    while (generator->diggers_count > generator->saved_diggers_count) {
        int last = generator->diggers_count - 1;
        digger_free(generator->diggers[last]);
        --generator->diggers_count;
    }
    for (int i = 0; i < generator->saved_diggers_count; ++i) {
        *(generator->diggers[i]) = generator->saved_diggers[i];
    }
    
    for (int i = 0; i < generator->tiles_count; ++i) {
        tile_free(generator->tiles[i]);
    }
    generator->tiles_count = 0;
}


void
generator_set_wall(struct generator *generator,
                 struct point point,
                 enum direction direction,
                 enum wall_type wall_type)
{
    struct tile *tile;
    switch (direction) {
        case direction_north:
            tile = generator_tile_at(generator, point_north(point));
            tile->walls.south = wall_type;
            break;
        case direction_south:
            tile = generator_tile_at(generator, point);
            tile->walls.south = wall_type;
            break;
        case direction_east:
            tile = generator_tile_at(generator, point_east(point));
            tile->walls.west = wall_type;
            break;
        case direction_west:
            tile = generator_tile_at(generator, point);
            tile->walls.west = wall_type;
            break;
        default:
            fail("Unrecognized direction %i", direction);
            break;
    }
}


void
generator_set_walls(struct generator *generator,
                    struct box box,
                    enum wall_type wall_type)
{
    struct point end = box_end_point(box);
    
    for (int i = 0; i < box.size.width; ++i) {
        int x = box.origin.x + i;
        
        struct point point = point_make(x, box.origin.y, box.origin.z);
        generator_set_wall(generator, point, direction_south, wall_type);
        
        point = point_make(x, end.y, box.origin.z);
        generator_set_wall(generator, point, direction_south, wall_type);
    }
    
    for (int j = 0; j < box.size.length; ++j) {
        int y = box.origin.y + j;
        
        struct point point = point_make(box.origin.x, y, box.origin.z);
        generator_set_wall(generator, point, direction_west, wall_type);
        
        point = point_make(end.x, y, box.origin.z);
        generator_set_wall(generator, point, direction_west, wall_type);
    }
}


struct tile *
generator_tile_at(struct generator *generator, struct point point)
{
    struct tile **tile = tile_find_in_array_sorted_by_point(generator->tiles,
                                                            generator->tiles_count,
                                                            point);
    if (tile) return *tile;
    
    struct tile *dungeon_tile = dungeon_tile_at(generator->dungeon, point);
    struct tile *copy = tile_alloc_copy(dungeon_tile);
    generator->tiles = tile_add_to_array_sorted_by_point(generator->tiles,
                                                         &generator->tiles_count,
                                                         copy);
    return copy;
}
