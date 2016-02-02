#ifndef FNF_DUNGEON_DIGGER_H_INCLUDED
#define FNF_DUNGEON_DIGGER_H_INCLUDED


#include "area_type.h"
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

void
digger_dig_area(struct digger *digger,
                uint32_t length,
                uint32_t width,
                uint32_t left_offset,
                enum area_type area_type);

void
digger_dig_chamber(struct digger *digger,
                   uint32_t length,
                   uint32_t width,
                   uint32_t left_offset);

void
digger_dig_intersection(struct digger *digger);

void
digger_dig_passage(struct digger *digger, uint32_t distance);


#endif
