#ifndef FNF_DUNGEON_GENERATOR_H_INCLUDED
#define FNF_DUNGEON_GENERATOR_H_INCLUDED


#include "area_type.h"
#include "box.h"
#include "direction.h"
#include "orientation.h"
#include "point.h"
#include "size.h"
#include "tile_type.h"


struct digger;
struct dungeon;
struct rnd;


struct generator {
    struct digger **diggers;
    int diggers_count;
    struct dungeon *dungeon;
    int iteration_count;
    struct size max_size;
    int padding;
    struct rnd *rnd;
};


struct generator *
generator_alloc(struct dungeon *dungeon, struct rnd *rnd);

void
generator_free(struct generator *generator);

void
generator_generate(struct generator *generator);

void
generator_generate_small(struct generator *generator);

struct digger *
generator_add_digger(struct generator *generator,
                     struct point point,
                     enum direction direction);

struct digger *
generator_copy_digger(struct generator *generator, struct digger *digger);

void
generator_delete_digger(struct generator *generator, struct digger *digger);

struct area *
generator_add_area(struct generator *generator,
                   enum area_type area_type,
                   enum orientation orientation,
                   struct box box,
                   enum tile_type tile_type);


#endif
