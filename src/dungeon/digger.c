#include "digger.h"

#include "common/alloc_or_die.h"


struct digger *
digger_alloc(struct dungeon_generator *generator,
             struct point point,
             enum direction direction,
             dig_fn *dig)
{
    struct digger *digger = malloc_or_die(sizeof(struct digger));
    digger->generator = generator;
    digger->point = point;
    digger->direction = direction;
    digger->dig = dig;
    return digger;
}


struct digger *
digger_alloc_copy(struct digger const *digger)
{
    return digger_alloc(digger->generator,
                        digger->point,
                        digger->direction,
                        digger->dig);
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
