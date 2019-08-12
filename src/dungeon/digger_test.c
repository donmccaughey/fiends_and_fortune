#include "digger.h"

#include <assert.h>
#include <base/base.h>
#include "dungeon.h"
#include "generator.h"


void
digger_test(void);


static void
digger_alloc_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(1, 2, 3), direction_west);
    assert(digger);
    assert(generator == digger->generator);
    assert(point_equals(point_make(1, 2, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_move_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(1, 2, 3), direction_west);
    assert(point_equals(point_make(1, 2, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 1, direction_north);
    assert(point_equals(point_make(1, 3, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 2, direction_east);
    assert(point_equals(point_make(3, 3, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 4, direction_south);
    assert(point_equals(point_make(3, -1, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 3, direction_west);
    assert(point_equals(point_make(0, -1, 3), digger->point));
    assert(direction_west == digger->direction);


    digger_move(digger, 2, direction_northeast);
    assert(point_equals(point_make(2, 1, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 5, direction_northwest);
    assert(point_equals(point_make(-3, 6, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 3, direction_southeast);
    assert(point_equals(point_make(0, 3, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_move(digger, 1, direction_southwest);
    assert(point_equals(point_make(-1, 2, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


void
digger_test(void)
{
    digger_alloc_test();
    digger_move_test();
}
