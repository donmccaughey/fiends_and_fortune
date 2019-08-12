#include "digger.h"

#include <assert.h>
#include <base/base.h>
#include "dungeon.h"
#include "generator.h"
#include "tile.h"


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


static void
digger_spin_90_degrees_left_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_spin_90_degrees_left(digger);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_south == digger->direction);

    digger->direction = direction_southeast;
    digger_spin_90_degrees_left(digger);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_northeast == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_spin_90_degrees_right_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    digger_spin_90_degrees_right(digger);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_north == digger->direction);

    digger->direction = direction_southeast;
    digger_spin_90_degrees_right(digger);
    assert(point_equals(point_make(0, 0, 1), digger->point));
    assert(direction_southwest == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_turn_90_degrees_left_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    // 1  .  .  .
    // 0  .  a  *
    //-1  .  .  b
    //   -1  0  1
    digger_turn_90_degrees_left(digger);
    assert(point_equals(point_make(1, -1, 1), digger->point));
    assert(direction_south == digger->direction);

    // 2   .  b  .
    // 1   *  .  .
    // 0   .  a  .
    //    -1  0  1
    digger->point = point_make(0, 0, 1);
    digger->direction = direction_southeast;
    digger_turn_90_degrees_left(digger);
    assert(point_equals(point_make(0, 2, 1), digger->point));
    assert(direction_northeast == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_turn_90_degrees_right_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    // 1  .  .  b
    // 0  .  a  *
    //-1  .  .  .
    //   -1  0  1
    digger_turn_90_degrees_right(digger);
    assert(point_equals(point_make(1, 1, 1), digger->point));
    assert(direction_north == digger->direction);

    // 2  .  .  .
    // 1  .  *  .
    // 0  b  .  a
    //   -2 -1  0
    digger->point = point_make(0, 0, 1);
    digger->direction = direction_southeast;
    digger_turn_90_degrees_right(digger);
    assert(point_equals(point_make(-2, 0, 1), digger->point));
    assert(direction_southwest == digger->direction);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_starting_stairs_test(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    assert(0 == generator->areas_count);
    assert(0 == generator->tiles_count);

    digger_dig_starting_stairs(digger);

    assert(1 == generator->areas_count);
    assert(area_type_stairs_up == generator->areas[0]->type);
    assert(area_features_none == generator->areas[0]->features);
    assert(direction_east == generator->areas[0]->direction);
    struct box expected_box = box_make(point_make(-1, 0, 1), size_make(2, 1, 1));
    assert(box_equals(expected_box, generator->areas[0]->box));

    // 1  f  f  f  f  .
    // 0  f  s  s  f  .
    //-1  .  f  f  f  .
    //   -2 -1  0  1  2
    assert(11 == generator->tiles_count);

    assert(point_equals(point_make(-1, -1, 1), generator->tiles[0]->point));
    assert(tile_type_filled == generator->tiles[0]->type);
    assert(wall_type_none == generator->tiles[0]->walls.south);
    assert(wall_type_none == generator->tiles[0]->walls.west);

    assert(point_equals(point_make(0, -1, 1), generator->tiles[1]->point));
    assert(tile_type_filled == generator->tiles[1]->type);
    assert(wall_type_none == generator->tiles[1]->walls.south);
    assert(wall_type_none == generator->tiles[1]->walls.west);

    assert(point_equals(point_make(1, -1, 1), generator->tiles[2]->point));
    assert(tile_type_filled == generator->tiles[2]->type);
    assert(wall_type_none == generator->tiles[2]->walls.south);
    assert(wall_type_none == generator->tiles[2]->walls.west);


    assert(point_equals(point_make(-2, 0, 1), generator->tiles[3]->point));
    assert(tile_type_filled == generator->tiles[3]->type);
    assert(wall_type_none == generator->tiles[3]->walls.south);
    assert(wall_type_none == generator->tiles[3]->walls.west);

    assert(point_equals(point_make(-1, 0, 1), generator->tiles[4]->point));
    assert(tile_type_stairs_up == generator->tiles[4]->type);
    assert(wall_type_solid == generator->tiles[4]->walls.south);
    assert(wall_type_none == generator->tiles[4]->walls.west);

    assert(point_equals(point_make(0, 0, 1), generator->tiles[5]->point));
    assert(tile_type_stairs_up == generator->tiles[5]->type);
    assert(wall_type_solid == generator->tiles[5]->walls.south);
    assert(wall_type_none == generator->tiles[5]->walls.west);

    assert(point_equals(point_make(1, 0, 1), generator->tiles[6]->point));
    assert(tile_type_filled == generator->tiles[6]->type);
    assert(wall_type_none == generator->tiles[6]->walls.south);
    assert(wall_type_solid == generator->tiles[6]->walls.west);


    assert(point_equals(point_make(-2, 1, 1), generator->tiles[7]->point));
    assert(tile_type_filled == generator->tiles[7]->type);
    assert(wall_type_none == generator->tiles[7]->walls.south);
    assert(wall_type_none == generator->tiles[7]->walls.west);

    assert(point_equals(point_make(-1, 1, 1), generator->tiles[8]->point));
    assert(tile_type_filled == generator->tiles[8]->type);
    assert(wall_type_solid == generator->tiles[8]->walls.south);
    assert(wall_type_none == generator->tiles[8]->walls.west);

    assert(point_equals(point_make(0, 1, 1), generator->tiles[9]->point));
    assert(tile_type_filled == generator->tiles[9]->type);
    assert(wall_type_solid == generator->tiles[9]->walls.south);
    assert(wall_type_none == generator->tiles[9]->walls.west);

    assert(point_equals(point_make(1, 1, 1), generator->tiles[10]->point));
    assert(tile_type_filled == generator->tiles[10]->type);
    assert(wall_type_none == generator->tiles[10]->walls.south);
    assert(wall_type_none == generator->tiles[10]->walls.west);

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
    digger_spin_90_degrees_left_test();
    digger_spin_90_degrees_right_test();
    digger_turn_90_degrees_left_test();
    digger_turn_90_degrees_right_test();
    digger_dig_starting_stairs_test();
}
