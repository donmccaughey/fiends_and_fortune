#ifndef FNF_DUNGEON_GENERATOR_H_INCLUDED
#define FNF_DUNGEON_GENERATOR_H_INCLUDED


#include <stddef.h>


struct digger;
struct dungeon;
struct rnd;


struct generator {
    struct dungeon *dungeon;
    struct rnd *rnd;
    struct digger **diggers;
    int diggers_count;
    int iteration_count;
};


struct generator *
generator_alloc(struct dungeon *dungeon, struct rnd *rnd);

void
generator_free(struct generator *generator);

void
generator_generate(struct generator *generator);

void
generator_generate_small(struct generator *generator);


#endif
