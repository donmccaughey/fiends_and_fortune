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


static void
digger_move_forward_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_move_forward(digger, 3);
    assert(point_equals(point_make(-3, 0, 1), digger->point));
    assert(direction_west == digger->direction);

    digger_move_forward(digger, 1);
    assert(point_equals(point_make(-4, 0, 1), digger->point));
    assert(direction_west == digger->direction);

    digger->direction = direction_north;
    digger_move_forward(digger, 2);
    assert(point_equals(point_make(-4, 2, 1), digger->point));
    assert(direction_north == digger->direction);

    digger->direction = direction_northeast;
    digger_move_forward(digger, 1);
    assert(point_equals(point_make(-3, 3, 1), digger->point));
    assert(direction_northeast == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_move_backward_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_move_backward(digger, 3);
    assert(point_equals(point_make(3, 0, 1), digger->point));
    assert(direction_west == digger->direction);

    digger_move_backward(digger, 1);
    assert(point_equals(point_make(4, 0, 1), digger->point));
    assert(direction_west == digger->direction);

    digger->direction = direction_north;
    digger_move_backward(digger, 2);
    assert(point_equals(point_make(4, -2, 1), digger->point));
    assert(direction_north == digger->direction);

    digger->direction = direction_northeast;
    digger_move_backward(digger, 1);
    assert(point_equals(point_make(3, -3, 1), digger->point));
    assert(direction_northeast == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_move_left_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_move_left(digger, 3);
    assert(point_equals(point_make(0, -3, 1), digger->point));
    assert(direction_west == digger->direction);

    digger_move_left(digger, 1);
    assert(point_equals(point_make(0, -4, 1), digger->point));
    assert(direction_west == digger->direction);

    digger->direction = direction_north;
    digger_move_left(digger, 2);
    assert(point_equals(point_make(-2, -4, 1), digger->point));
    assert(direction_north == digger->direction);

    digger->direction = direction_northeast;
    digger_move_left(digger, 1);
    assert(point_equals(point_make(-3, -3, 1), digger->point));
    assert(direction_northeast == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_move_right_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_move_right(digger, 3);
    assert(point_equals(point_make(0, 3, 1), digger->point));
    assert(direction_west == digger->direction);

    digger_move_right(digger, 1);
    assert(point_equals(point_make(0, 4, 1), digger->point));
    assert(direction_west == digger->direction);

    digger->direction = direction_north;
    digger_move_right(digger, 2);
    assert(point_equals(point_make(2, 4, 1), digger->point));
    assert(direction_north == digger->direction);

    digger->direction = direction_northeast;
    digger_move_right(digger, 1);
    assert(point_equals(point_make(3, 3, 1), digger->point));
    assert(direction_northeast == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_ascend_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 4), direction_west);

    digger_ascend(digger, 1);
    assert(point_equals(point_make(0, 0, 3), digger->point));
    assert(direction_west == digger->direction);

    digger_ascend(digger, 2);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_west == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_descend_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_descend(digger, 1);
    assert(point_equals(point_make(0, 0, 2), digger->point));
    assert(direction_west == digger->direction);

    digger_descend(digger, 3);
    assert(point_equals(point_make(0, 0, 5), digger->point));
    assert(direction_west == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_spin_180_degrees_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);

    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_spin_180_degrees(digger);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_east == digger->direction);

    digger->direction = direction_southeast;
    digger_spin_180_degrees(digger);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_northwest == digger->direction);

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
    digger_move_forward_test();
    digger_move_backward_test();
    digger_move_left_test();
    digger_move_right_test();
    digger_ascend_test();
    digger_descend_test();
    digger_spin_180_degrees_test();
}
