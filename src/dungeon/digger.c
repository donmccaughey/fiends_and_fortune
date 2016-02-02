#include "digger.h"

#include "common/alloc_or_die.h"

#include "dungeon_generator.h"


struct digger *
digger_alloc(struct point point, enum direction direction)
{
    struct digger *digger = calloc_or_die(1, sizeof(struct digger));
    digger->point = point;
    digger->direction = direction;
    return digger;
}


void
digger_free(struct digger *digger)
{
    free_or_die(digger);
}


void
digger_turn_90_degrees_left(struct digger *digger)
{
    digger->point = point_rotate_90_degrees_left(digger->point, digger->direction);
    digger->direction = direction_90_degrees_left(digger->direction);
}


void
digger_turn_90_degrees_right(struct digger *digger)
{
    digger->point = point_rotate_90_degrees_right(digger->point, digger->direction);
    digger->direction = direction_90_degrees_right(digger->direction);
}
