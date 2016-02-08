#ifndef FNF_DUNGEON_DIGGER_H_INCLUDED
#define FNF_DUNGEON_DIGGER_H_INCLUDED


#include "area_type.h"
#include "direction.h"
#include "point.h"
#include "wall_type.h"


struct area;
struct digger;
struct dungeon_generator;


struct digger {
    struct dungeon_generator *generator;
    struct point point;
    enum direction direction;
};


void
digger_move(struct digger *digger, int steps, enum direction direction);

void
digger_turn_90_degrees_left(struct digger *digger);

void
digger_turn_90_degrees_right(struct digger *digger);

struct area *
digger_dig_area(struct digger *digger,
                int length,
                int width,
                int left_offset,
                enum wall_type entrance_type,
                enum area_type area_type);

struct area *
digger_dig_chamber(struct digger *digger,
                   int length,
                   int width,
                   int left_offset);

struct area *
digger_dig_intersection(struct digger *digger);

struct area *
digger_dig_passage(struct digger *digger, int distance);

void
digger_periodic_check(struct digger *digger);

void
digger_generate_chamber(struct digger *digger);

void
digger_generate_side_passage(struct digger *digger);

void
digger_generate_turn(struct digger *digger);


#endif
