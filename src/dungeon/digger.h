#ifndef FNF_DUNGEON_DIGGER_H_INCLUDED
#define FNF_DUNGEON_DIGGER_H_INCLUDED


#include "direction.h"
#include "point.h"


struct digger;
struct dungeon_generator;


struct digger {
    struct dungeon_generator *generator;
    struct point point;
    enum direction direction;
};


struct digger *
digger_alloc(struct point point, enum direction direction);

void
digger_free(struct digger *digger);

void
digger_turn_90_degrees_left(struct digger *digger);

void
digger_turn_90_degrees_right(struct digger *digger);


#endif
