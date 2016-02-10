#ifndef FNF_DUNGEON_DUNGEON_GENERATOR_H_INCLUDED
#define FNF_DUNGEON_DUNGEON_GENERATOR_H_INCLUDED


#include "direction.h"
#include "point.h"
#include "size.h"


struct digger;
struct dungeon;
struct rnd;


struct dungeon_generator {
    int buffer;
    struct digger **diggers;
    int diggers_count;
    struct dungeon *dungeon;
    int iteration_count;
    struct size max_size;
    struct rnd *rnd;
};


struct dungeon_generator *
dungeon_generator_alloc(struct dungeon *dungeon, struct rnd *rnd);

void
dungeon_generator_free(struct dungeon_generator *generator);

void
dungeon_generator_generate(struct dungeon_generator *generator);

void
dungeon_generator_generate_small(struct dungeon_generator *generator);

struct digger *
dungeon_generator_add_digger(struct dungeon_generator *generator,
                             struct point point,
                             enum direction direction);

struct digger *
dungeon_generator_copy_digger(struct dungeon_generator *generator,
                              struct digger *digger);

void
dungeon_generator_delete_digger(struct dungeon_generator *generator,
                                struct digger *digger);


#endif
