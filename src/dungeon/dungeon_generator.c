#include "dungeon_generator.h"

#include <assert.h>

#include "common/alloc_or_die.h"

#include "digger.h"
#include "dungeon.h"
#include "tile.h"
#include "tiles.h"


struct dungeon_generator *
dungeon_generator_alloc(struct dungeon *dungeon, struct rnd *rnd)
{
    struct dungeon_generator *generator = calloc_or_die(1, sizeof(struct dungeon_generator));
    generator->dungeon = dungeon;
    generator->rnd = rnd;
    generator->diggers = calloc_or_die(1, sizeof(struct digger *));
    return generator;
}


void
dungeon_generator_free(struct dungeon_generator *generator)
{
    for (int i = 0; i < generator->diggers_count; ++i) {
        digger_free(generator->diggers[i]);
    }
    free_or_die(generator->diggers);
    free_or_die(generator);
}


void
dungeon_generator_give_up_digger(struct dungeon_generator *generator,
                                 struct digger *digger)
{
    int index = -1;
    for (int i = 0; i < generator->diggers_count; ++i) {
        if (digger == generator->diggers[i]) {
            index = i;
            break;
        }
    }
    assert(index >= 0 && index < generator->diggers_count);
    generator->diggers[index] = NULL;
    digger->generator = NULL;
    
    int next_index = index + 1;
    int last_index = generator->diggers_count - 1;
    if (next_index < last_index) {
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
dungeon_generator_generate(struct dungeon_generator *generator)
{
    int const max_interation_count = 7;
    
    struct digger *digger = digger_alloc(point_make(0, 0, 1), North);
    dungeon_generator_take_on_digger(generator, digger);
    digger_dig_passage(digger, 2);
    
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
dungeon_generator_generate_small(struct dungeon_generator *generator)
{
    struct digger *digger = digger_alloc(point_make(0, 0, 1), North);
    dungeon_generator_take_on_digger(generator, digger);
    
    digger_dig_passage(digger, 2);
    digger_dig_chamber(digger, 5, 3, 1);
    
    /* from entry chamber, north west exit */
    struct digger *nw_digger = digger_copy(digger);
    digger_turn_90_degrees_left(nw_digger);
    digger_move(nw_digger, 1, South);
    digger_move(nw_digger, 1, West);
    digger_dig_passage(nw_digger, 2);
    digger_turn_90_degrees_right(nw_digger);
    digger_dig_passage(nw_digger, 7);
    digger_dig_chamber(nw_digger, 3, 2, 1);
    
    /* from entry chamber, south west exit */
    struct digger *sw_digger = digger_copy(digger);
    digger_turn_90_degrees_left(sw_digger);
    digger_move(sw_digger, 3, South);
    digger_move(sw_digger, 1, West);
    digger_dig_passage(sw_digger, 2);
    digger_turn_90_degrees_left(sw_digger);
    digger_dig_passage(sw_digger, 2);
    digger_turn_90_degrees_right(sw_digger);
    digger_dig_passage(sw_digger, 2);
    digger_turn_90_degrees_right(sw_digger);
    digger_dig_passage(sw_digger, 2);
    digger_dig_chamber(sw_digger, 2, 2, 1);
    digger_move(sw_digger, 1, West);
    digger_dig_passage(sw_digger, 3);
    digger_dig_chamber(sw_digger, 2, 3, 1);
    
    /* from entry chamber, south east exit */
    struct digger *se_digger = digger_copy(digger);
    digger_turn_90_degrees_right(se_digger);
    digger_move(se_digger, 3, South);
    digger_move(se_digger, 1, East);
    digger_dig_passage(se_digger, 1);
    digger_dig_chamber(se_digger, 6, 4, 0);
    // TODO: add digger_fill()
    struct tile *tile = tiles_find_tile_at(generator->dungeon->tiles, point_make(5, 2, 1));
    tiles_remove_tile(generator->dungeon->tiles, tile);
    tile_free(tile);
    
    /* from entry chamber, north exit */
    digger_dig_passage(digger, 8);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 8);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 3);
    digger_dig_chamber(digger, 3, 4, 0);
    digger_move(digger, 3, West);
    digger_dig_passage(digger, 3);
    digger_turn_90_degrees_right(digger);
    digger_dig_passage(digger, 3);
    
//    struct point point = digger->point;
//    point = passage(generator->dungeon, point, 7, North);
//    point = passage(generator->dungeon, point, 8, East);
//    point = passage(generator->dungeon, point, 4, South);
//    
//    point = chamber(generator->dungeon, point, 3, 4, South, 0);
//    point = point_move(point, 3, West);
//    
//    point = passage(generator->dungeon, point, 2, South);
//    passage(generator->dungeon, point, 4, West);
}


void
dungeon_generator_take_on_digger(struct dungeon_generator *generator,
                                 struct digger *digger)
{
    int index = generator->diggers_count;
    ++generator->diggers_count;
    generator->diggers = reallocarray_or_die(generator->diggers,
                                             generator->diggers_count,
                                             sizeof(struct digger *));
    digger->generator = generator;
    generator->diggers[index] = digger;
}
