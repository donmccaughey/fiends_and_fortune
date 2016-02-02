#ifndef FNF_DUNGEON_DIGGER_H_INCLUDED
#define FNF_DUNGEON_DIGGER_H_INCLUDED


#include "direction.h"
#include "point.h"


struct digger;
struct dungeon_generator;


typedef void
dig_fn(struct digger *digger);


struct digger {
    struct dungeon_generator *generator;
    struct point point;
    enum direction direction;
    dig_fn *dig;
};


struct digger *
digger_alloc(struct dungeon_generator *generator,
             struct point point,
             enum direction direction,
             dig_fn *dig);

struct digger *
digger_alloc_copy(struct digger const *digger);

void
digger_free(struct digger *digger);

void
digger_turn_90_degrees_left(struct digger *digger);

void
digger_turn_90_degrees_right(struct digger *digger);


#endif
