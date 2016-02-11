#include "generator.h"

#include <assert.h>

#include "common/alloc_or_die.h"
#include "common/fail.h"
#include "common/rnd.h"

#include "area.h"
#include "digger.h"
#include "dungeon.h"
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
                   enum orientation orientation,
                   struct box box,
                   enum tile_type tile_type)
{
    struct area *area = area_alloc(area_type,
                                   orientation,
                                   box,
                                   tile_type);    
    generator_fill_box(generator, box, tile_type);
    generator_set_walls(generator, box, wall_type_solid);
    dungeon_add_area(generator->dungeon, area);
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
generator_alloc(struct dungeon *dungeon, struct rnd *rnd)
{
    struct generator *generator = calloc_or_die(1, sizeof(struct generator));
    generator->dungeon = dungeon;
    generator->rnd = rnd;
    
    generator->padding = rnd_next_uniform_value(rnd, 2);
    generator->max_size = size_make(20, 20, 5);
    
    generator->diggers = calloc_or_die(1, sizeof(struct digger *));
    return generator;
}


struct box
generator_box_for_level(struct generator *generator, int level)
{
    return dungeon_box_for_level(generator->dungeon, level);
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
                   enum tile_type tile_type)
{
    struct box padded_box = box_expand(box, size_make(1, 1, 0));
    struct tile **tiles = dungeon_alloc_tiles_for_box(generator->dungeon,
                                                      padded_box);
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


void
generator_free(struct generator *generator)
{
    for (int i = 0; i < generator->diggers_count; ++i) {
        digger_free(generator->diggers[i]);
    }
    free_or_die(generator->diggers);
    free_or_die(generator);
}


void
generator_generate(struct generator *generator)
{
    int const max_interation_count = 100;
    
    struct digger *digger = generator_add_digger(generator,
                                                 point_make(0, 0, 1),
                                                 direction_north);
    digger_dig_area(digger, 2, 1, 0, wall_type_solid, area_type_passage);
    
    while (generator->diggers_count && generator->iteration_count < max_interation_count) {
        struct digger **diggers = arraydup_or_die(generator->diggers,
                                                  generator->diggers_count,
                                                  sizeof(struct digger *));
        int count = generator->diggers_count;
        for (int i = 0; i < count; ++i) {
            digger_periodic_check(diggers[i]);
        }
        free_or_die(diggers);
        ++generator->iteration_count;
    }
}


void
generator_generate_small(struct generator *generator)
{
    struct digger *digger = generator_add_digger(generator,
                                                 point_make(0, 0, 1),
                                                 direction_north);
    
    digger_dig_area(digger, 2, 1, 0, wall_type_solid, area_type_passage);
    digger_dig_chamber(digger, 5, 3, 1, wall_type_none);
    
    // from entry chamber, north west exit
    struct digger *nw_digger = generator_copy_digger(generator, digger);
    digger_turn_90_degrees_left(nw_digger);
    digger_move(nw_digger, 1, direction_south);
    digger_move(nw_digger, 1, direction_west);
    digger_dig_passage(nw_digger, 2, wall_type_none);
    digger_turn_90_degrees_right(nw_digger);
    digger_dig_passage(nw_digger, 6, wall_type_none);
    digger_dig_chamber(nw_digger, 3, 2, 1, wall_type_none);
    
    // from entry chamber, south west exit
    struct digger *sw_digger = generator_copy_digger(generator, digger);
    digger_turn_90_degrees_left(sw_digger);
    digger_move(sw_digger, 3, direction_south);
    digger_move(sw_digger, 1, direction_west);
    digger_dig_passage(sw_digger, 2, wall_type_door);
    digger_turn_90_degrees_left(sw_digger);
    digger_dig_passage(sw_digger, 2, wall_type_none);
    digger_turn_90_degrees_right(sw_digger);
    digger_dig_passage(sw_digger, 2, wall_type_none);
    digger_turn_90_degrees_right(sw_digger);
    digger_dig_passage(sw_digger, 2, wall_type_door);
    digger_dig_chamber(sw_digger, 2, 2, 1, wall_type_none);
    digger_move(sw_digger, 1, direction_west);
    digger_dig_passage(sw_digger, 3, wall_type_door);
    digger_dig_chamber(sw_digger, 2, 3, 1, wall_type_none);
    
    // from entry chamber, south east exit
    struct digger *se_digger = generator_copy_digger(generator, digger);
    digger_turn_90_degrees_right(se_digger);
    digger_move(se_digger, 3, direction_south);
    digger_move(se_digger, 1, direction_east);
    digger_dig_passage(se_digger, 1, wall_type_none);
    digger_dig_chamber(se_digger, 6, 4, 0, wall_type_none);
    // fill in one tile in chamber
    struct box box = box_make(point_make(5, 2, 1), size_make_unit());
    generator_fill_box(generator, box, tile_type_solid);
    
    // from entry chamber, north exit
    digger_dig_passage(digger, 8, wall_type_none);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 8, wall_type_none);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 3, wall_type_none);
    digger_dig_chamber(digger, 3, 4, 0, wall_type_none);
    digger_move(digger, 3, direction_west);
    digger_dig_passage(digger, 3, wall_type_none);
    digger_turn_90_degrees_right(digger);
    // dig connecting passage without constraints to make looping passage
    digger_dig_area(digger, 3, 1, 0, wall_type_none, area_type_passage);
    struct tile *tile = dungeon_tile_at(generator->dungeon, point_east(digger->point));
    tile->walls.west = wall_type_none;
}


bool
generator_is_box_excavated(struct generator *generator, struct box box)
{
    return dungeon_is_box_excavated(generator->dungeon, box);
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
            tile = dungeon_tile_at(generator->dungeon, point_north(point));
            tile->walls.south = wall_type;
            break;
        case direction_south:
            tile = dungeon_tile_at(generator->dungeon, point);
            tile->walls.south = wall_type;
            break;
        case direction_east:
            tile = dungeon_tile_at(generator->dungeon, point_east(point));
            tile->walls.west = wall_type;
            break;
        case direction_west:
            tile = dungeon_tile_at(generator->dungeon, point);
            tile->walls.west = wall_type;
            break;
        default:
            fail("Unrecognized direction %i", direction);
            break;
    }
}


void
generator_set_walls(struct generator *dungeon,
                    struct box box,
                    enum wall_type wall_type)
{
    struct point end = box_end_point(box);
    
    for (int i = 0; i < box.size.width; ++i) {
        int x = box.origin.x + i;
        
        struct point point = point_make(x, box.origin.y, box.origin.z);
        generator_set_wall(dungeon, point, direction_south, wall_type);
        
        point = point_make(x, end.y, box.origin.z);
        generator_set_wall(dungeon, point, direction_south, wall_type);
    }
    
    for (int j = 0; j < box.size.length; ++j) {
        int y = box.origin.y + j;
        
        struct point point = point_make(box.origin.x, y, box.origin.z);
        generator_set_wall(dungeon, point, direction_west, wall_type);
        
        point = point_make(end.x, y, box.origin.z);
        generator_set_wall(dungeon, point, direction_west, wall_type);
    }
}


struct tile *
generator_tile_at(struct generator *generator, struct point point)
{
    return dungeon_tile_at(generator->dungeon, point);
}
