#include "digger.h"

#include <assert.h>
#include <base/base.h>
#include "dungeon.h"
#include "generator.h"
#include "tile.h"
#include "tiles_thumbnail.h"


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

    digger_dig_starting_stairs(digger);

    assert(1 == generator->areas_count);
    assert(area_type_stairs_up == generator->areas[0]->type);
    assert(area_features_none == generator->areas[0]->features);
    assert(direction_east == generator->areas[0]->direction);
    struct box expected_box = box_make(point_make(-1, 0, 1), size_make(2, 1, 1));
    assert(box_equals(expected_box, generator->areas[0]->box));

    assert(11 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -2 -1  0  1 \n"
            " 1  n  n  n  n \n"
            " 0  n  e  e  n \n"
            "-1     n  n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -2 -1  0  1 \n"
            " 1  0  0  0  0 \n"
            " 0  0  0  0  0 \n"
            "-1     0  0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -2 -1  0  1 \n"
            " 1  :  :  :  : \n"
            " 0  :  ^  ^  : \n"
            "-1     :  :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -2 -1  0  1 \n"
            " 1 .. ._ ._ .. \n"
            " 0 .. ._ ._ |. \n"
            "-1    .. .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_10x10_chamber_north(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_north);

    struct area *area = digger_dig_area(digger, 1, 1, 0, wall_type_door, area_type_chamber);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_chamber == area->type);
    assert(area_features_none == area->features);
    assert(direction_north == area->direction);
    struct box expected_box = box_make(point_make(0, 0, 1), size_make(1, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(8 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -1  0  1 \n"
            " 1  n  n  n \n"
            " 0  n  n  n \n"
            "-1     n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -1  0  1 \n"
            " 1  0  0  0 \n"
            " 0  0  0  0 \n"
            "-1     0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -1  0  1 \n"
            " 1  :  :  : \n"
            " 0  :  .  : \n"
            "-1     :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -1  0  1 \n"
            " 1 .. ._ .. \n"
            " 0 .. |= |. \n"
            "-1    .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_10x10_chamber_south(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_south);

    struct area *area = digger_dig_area(digger, 1, 1, 0, wall_type_door, area_type_chamber);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_chamber == area->type);
    assert(area_features_none == area->features);
    assert(direction_south == area->direction);
    struct box expected_box = box_make(point_make(0, 0, 1), size_make(1, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(8 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -1  0  1 \n"
            " 1  n  n  n \n"
            " 0  n  s  n \n"
            "-1     n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -1  0  1 \n"
            " 1  0  0  0 \n"
            " 0  0  0  0 \n"
            "-1     0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -1  0  1 \n"
            " 1  :  :  : \n"
            " 0  :  .  : \n"
            "-1     :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -1  0  1 \n"
            " 1 .. .= .. \n"
            " 0 .. |_ |. \n"
            "-1    .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_10x10_chamber_east(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_east);

    struct area *area = digger_dig_area(digger, 1, 1, 0, wall_type_door, area_type_chamber);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_chamber == area->type);
    assert(area_features_none == area->features);
    assert(direction_east == area->direction);
    struct box expected_box = box_make(point_make(0, 0, 1), size_make(1, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(8 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -1  0  1 \n"
            " 1  n  n  n \n"
            " 0  n  e  n \n"
            "-1     n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -1  0  1 \n"
            " 1  0  0  0 \n"
            " 0  0  0  0 \n"
            "-1     0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -1  0  1 \n"
            " 1  :  :  : \n"
            " 0  :  .  : \n"
            "-1     :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -1  0  1 \n"
            " 1 .. ._ .. \n"
            " 0 .. ]_ |. \n"
            "-1    .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_10x10_chamber_west(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_west);

    struct area *area = digger_dig_area(digger, 1, 1, 0, wall_type_door, area_type_chamber);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_chamber == area->type);
    assert(area_features_none == area->features);
    assert(direction_west == area->direction);
    struct box expected_box = box_make(point_make(0, 0, 1), size_make(1, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(8 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -1  0  1 \n"
            " 1  n  n  n \n"
            " 0  n  w  n \n"
            "-1     n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -1  0  1 \n"
            " 1  0  0  0 \n"
            " 0  0  0  0 \n"
            "-1     0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -1  0  1 \n"
            " 1  :  :  : \n"
            " 0  :  .  : \n"
            "-1     :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -1  0  1 \n"
            " 1 .. ._ .. \n"
            " 0 .. |_ ]. \n"
            "-1    .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_30x20_room(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_south);

    struct area *area = digger_dig_area(digger, 2, 3, 0, wall_type_door, area_type_room);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_room == area->type);
    assert(area_features_none == area->features);
    assert(direction_south == area->direction);
    struct box expected_box = box_make(point_make(-2, -1, 1), size_make(3, 2, 1));
    assert(box_equals(expected_box, area->box));

    assert(19 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -3 -2 -1  0  1 \n"
            " 1  n  n  n  n  n \n"
            " 0  n  s  s  s  n \n"
            "-1  n  s  s  s  n \n"
            "-2     n  n  n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -3 -2 -1  0  1 \n"
            " 1  0  0  0  0  0 \n"
            " 0  0  0  0  0  0 \n"
            "-1  0  0  0  0  0 \n"
            "-2     0  0  0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -3 -2 -1  0  1 \n"
            " 1  :  :  :  :  : \n"
            " 0  :  .  .  .  : \n"
            "-1  :  .  .  .  : \n"
            "-2     :  :  :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -3 -2 -1  0  1 \n"
            " 1 .. ._ ._ .= .. \n"
            " 0 .. |. .. .. |. \n"
            "-1 .. |_ ._ ._ |. \n"
            "-2    .. .. .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_60_passage(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(-3, 0, 1), direction_east);

    struct area *area = digger_dig_area(digger, 6, 1, 0, wall_type_none, area_type_passage);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_passage == area->type);
    assert(area_features_none == area->features);
    assert(direction_east == area->direction);
    struct box expected_box = box_make(point_make(-3, 0, 1), size_make(6, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(23 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -4 -3 -2 -1  0  1  2  3 \n"
            " 1  n  n  n  n  n  n  n  n \n"
            " 0  n  e  e  e  e  e  e  n \n"
            "-1     n  n  n  n  n  n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -4 -3 -2 -1  0  1  2  3 \n"
            " 1  0  0  0  0  0  0  0  0 \n"
            " 0  0  0  0  0  0  0  0  0 \n"
            "-1     0  0  0  0  0  0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -4 -3 -2 -1  0  1  2  3 \n"
            " 1  :  :  :  :  :  :  :  : \n"
            " 0  :  .  .  .  .  .  .  : \n"
            "-1     :  :  :  :  :  :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -4 -3 -2 -1  0  1  2  3 \n"
            " 1 .. ._ ._ ._ ._ ._ ._ .. \n"
            " 0 .. ._ ._ ._ ._ ._ ._ |. \n"
            "-1    .. .. .. .. .. .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_stairs_up(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_east);

    struct area *area = digger_dig_area(digger, 2, 1, 0, wall_type_none, area_type_stairs_up);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_stairs_up == area->type);
    assert(area_features_none == area->features);
    assert(direction_east == area->direction);
    struct box expected_box = box_make(point_make(0, 0, 1), size_make(2, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(11 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -1  0  1  2 \n"
            " 1  n  n  n  n \n"
            " 0  n  e  e  n \n"
            "-1     n  n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -1  0  1  2 \n"
            " 1  0  0  0  0 \n"
            " 0  0  0  0  0 \n"
            "-1     0  0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -1  0  1  2 \n"
            " 1  :  :  :  : \n"
            " 0  :  ^  ^  : \n"
            "-1     :  :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -1  0  1  2 \n"
            " 1 .. ._ ._ .. \n"
            " 0 .. ._ ._ |. \n"
            "-1    .. .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_stairs_down(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_east);

    struct area *area = digger_dig_area(digger, 2, 1, 0, wall_type_none, area_type_stairs_down);

    assert(area);
    assert(1 == generator->areas_count);
    assert(area == generator->areas[0]);
    assert(area_type_stairs_down == area->type);
    assert(area_features_none == area->features);
    assert(direction_east == area->direction);
    struct box expected_box = box_make(point_make(0, 0, 1), size_make(2, 1, 1));
    assert(box_equals(expected_box, area->box));

    assert(11 == generator->tiles_count);
    char *thumbnail;

    char const *expected_directions =
            "   -1  0  1  2 \n"
            " 1  n  n  n  n \n"
            " 0  n  e  e  n \n"
            "-1     n  n  n \n";
    thumbnail = tiles_thumbnail_directions_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_directions, thumbnail));
    free_or_die(thumbnail);

    char const *expected_features =
            "   -1  0  1  2 \n"
            " 1  0  0  0  0 \n"
            " 0  0  0  0  0 \n"
            "-1     0  0  0 \n";
    thumbnail = tiles_thumbnail_features_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_features, thumbnail));
    free_or_die(thumbnail);

    char const *expected_types =
            "   -1  0  1  2 \n"
            " 1  :  :  :  : \n"
            " 0  :  v  v  : \n"
            "-1     :  :  : \n";
    thumbnail = tiles_thumbnail_types_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_types, thumbnail));
    free_or_die(thumbnail);

    char const *expected_walls =
            "   -1  0  1  2 \n"
            " 1 .. ._ ._ .. \n"
            " 0 .. ._ ._ |. \n"
            "-1    .. .. .. \n";
    thumbnail = tiles_thumbnail_walls_alloc(generator->tiles, generator->tiles_count);
    assert(str_eq(expected_walls, thumbnail));
    free_or_die(thumbnail);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_for_overlapping_areas(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(-3, 0, 1), direction_east);

    struct area *area1 = digger_dig_area(digger, 6, 1, 0, wall_type_none, area_type_passage);

    assert(area1);
    assert(1 == generator->areas_count);
    struct box expected_box = box_make(point_make(-3, 0, 1), size_make(6, 1, 1));
    assert(box_equals(expected_box, generator->areas[0]->box));

    digger_move(digger, 3, direction_west);
    digger_move(digger, 3, direction_north);
    digger_spin_90_degrees_right(digger);
    digger_dig_area(digger, 6, 1, 0, wall_type_none, area_type_passage);

    struct area *area2 = digger_dig_area(digger, 6, 1, 0, wall_type_none, area_type_passage);

    assert(!area2);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_when_outside_dungeon_max_width(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    dungeon_options->max_size = size_make(10, 10, 1);
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_north);
    digger_dig_area(digger, 1, 1, 0, wall_type_solid, area_type_chamber);
    digger_move(digger, 10, direction_east);
    digger_move(digger, 10, direction_north);
    digger_dig_area(digger, 1, 1, 0, wall_type_solid, area_type_chamber);

    digger_move(digger, 1, direction_east);
    struct area *area = digger_dig_area(digger, 1, 1, 0, wall_type_solid, area_type_chamber);

    assert(!area);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_when_outside_dungeon_max_height(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    dungeon_options->max_size = size_make(10, 10, 1);
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_north);
    digger_dig_area(digger, 1, 1, 0, wall_type_solid, area_type_chamber);
    digger_move(digger, 10, direction_east);
    digger_move(digger, 10, direction_north);
    digger_dig_area(digger, 1, 1, 0, wall_type_solid, area_type_chamber);

    digger_move(digger, 1, direction_north);
    struct area *area = digger_dig_area(digger, 1, 1, 0, wall_type_solid, area_type_chamber);

    assert(!area);

    digger_free(digger);
    generator_free(generator);
    dungeon_options_free(dungeon_options);
    dungeon_free(dungeon);
}


static void
digger_dig_area_test_when_bigger_than_dungeon_max(void)
{
    struct dungeon *dungeon = dungeon_alloc();
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    dungeon_options->max_size = size_make(10, 10, 1);
    struct generator *generator = generator_alloc(dungeon, global_rnd, dungeon_options, NULL, NULL);
    struct digger *digger = digger_alloc(generator, point_make(0, 0, 1), direction_north);

    struct area *area = digger_dig_area(digger, 11, 1, 0, wall_type_solid, area_type_passage);

    assert(!area);

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
    digger_dig_area_test_for_10x10_chamber_north();
    digger_dig_area_test_for_10x10_chamber_south();
    digger_dig_area_test_for_10x10_chamber_east();
    digger_dig_area_test_for_10x10_chamber_west();
    digger_dig_area_test_for_30x20_room();
    digger_dig_area_test_for_60_passage();
    digger_dig_area_test_for_stairs_up();
    digger_dig_area_test_for_stairs_down();
    digger_dig_area_test_for_overlapping_areas();
    digger_dig_area_test_when_outside_dungeon_max_width();
    digger_dig_area_test_when_outside_dungeon_max_height();
    digger_dig_area_test_when_bigger_than_dungeon_max();
}
