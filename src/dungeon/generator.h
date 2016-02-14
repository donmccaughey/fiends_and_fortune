#ifndef FNF_DUNGEON_GENERATOR_H_INCLUDED
#define FNF_DUNGEON_GENERATOR_H_INCLUDED


#include <stdbool.h>

#include "area_type.h"
#include "box.h"
#include "direction.h"
#include "tile_type.h"
#include "wall_type.h"


struct area;
struct digger;
struct dungeon;
struct rnd;
struct tile;


struct generator {
    struct area **areas;
    int areas_count;
    struct digger **diggers;
    int diggers_count;
    struct dungeon *dungeon;
    int iteration_count;
    struct size max_size;
    int padding;
    struct rnd *rnd;
    struct digger *saved_diggers;
    int saved_diggers_count;
    struct tile **tiles;
    int tiles_count;
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

void
generator_commit(struct generator *generator);

void
generator_rollback(struct generator *generator);

bool
generator_is_box_excavated(struct generator *generator, struct box box);

struct box
generator_box_for_level(struct generator *generator, int level);

struct tile *
generator_tile_at(struct generator *generator, struct point point);

struct area *
generator_add_area(struct generator *generator,
                   enum area_type area_type,
                   enum direction direction,
                   struct box box,
                   enum tile_type tile_type);

void
generator_fill_box(struct generator *generator,
                   struct box box,
                   enum direction direction,
                   enum tile_type tile_type);

void
generator_set_wall(struct generator *generator,
                   struct point point,
                   enum direction direction,
                   enum wall_type wall_type);

void
generator_set_walls(struct generator *generator,
                    struct box box,
                    enum wall_type wall_type);


#endif
